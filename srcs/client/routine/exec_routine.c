/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 11:41:20 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/16 12:55:16 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static t_vector		*get_trame(t_cmd *cmd, const uint8_t flag)
{
	static	t_builtin	builtin[] = {blt_add, blt_avail, blt_clear, blt_exit,
									blt_fg, blt_help, blt_maintail, blt_open,
									blt_pid, blt_quit, blt_reload, blt_remove,
									blt_reread, blt_restart, blt_shutdown,
									blt_signal, blt_start, blt_status, blt_stop,
									blt_tail, blt_update, blt_version};
	t_vector			*trame;

	trame = builtin[cmd->type](cmd);
	if (g_env->opt.mask & OPT_DEBUG && trame != NULL && flag != KEEP_FEEDBACK)
		debug_print_bytecode(trame);
	return (trame);
}

static int			is_socket_cleared(int fd, uint8_t flag)
{
	char		c;
	bool		ret;

	ret = true;
	while (read(fd, &c, 1) > 0)
		ret = false;
	vct_creadline(NULL, CLEANUP, EOT);
	if (ret == false && flag == BEFORE)
		dprintf(STDERR_FILENO,
			"ERROR: cmd failed cause the socket was not empty...\n");
	return (ret);
}

static t_vector		*send_and_receive(t_vector *trame, const uint8_t flag)
{
	t_vector		*feedback;

	feedback = NULL;
	if (is_socket_cleared(g_env->unix_socket, BEFORE) == false)
		return (NULL);
	if (try_to_send_trame(g_env->unix_socket, trame,
							TO_PRINT, ft_dprintf) == SUCCESS)
	{
		feedback = get_feedback();
		if (feedback == NULL && flag == DEL_FEEDBACK)
		{
			dprintf(STDERR_FILENO, "Got no feedback from daemon!\n");
			g_env->sigint = 0;
			is_socket_cleared(g_env->unix_socket, AFTER);
			return (NULL);
		}
		if (g_env->flag_exec != 0 && vct_getlastchar(feedback) == ETX)
			exec_tail(g_env->flag_exec, feedback);
		else if (flag == DEL_FEEDBACK)
			vct_print_fd(feedback, STDOUT_FILENO);
	}
	is_socket_cleared(g_env->unix_socket, AFTER);
	return (feedback);
}

t_vector			*routine(t_vector *line, const uint8_t flag)
{
	t_vector		*trame;
	t_vector		*feedback;

	g_env->flag_exec = 0;
	g_env->cmd = get_cmd(line);
	trame = NULL;
	feedback = NULL;
	if (g_env->cmd != NULL)
	{
		trame = get_trame(g_env->cmd, flag);
		if (trame != NULL)
			feedback = send_and_receive(trame, flag);
		if (feedback != NULL && g_env->cmd->type == RELOAD)
			restart_connection();
	}
	vct_del(&trame);
	reset_cmd();
	if (flag == DEL_FEEDBACK)
		vct_del(&feedback);
	return (feedback);
}

void				read_cmd(void)
{
	t_vector	*line;
	t_vector	*split;
	int			ret;

	line = vct_new(DFL_VCT_SIZE);
	get_status(DEL_FEEDBACK);
	while ((ret = tsk_readline(line, STDIN_FILENO, g_env)) >= 0)
	{
		if (vct_apply(line, IS_SPACE) == false)
		{
			history(line, ADD_ENTRY | RESET);
			while ((split = vct_split(line, ";", NO_SEP)) != NULL)
			{
				routine(split, DEL_FEEDBACK);
				vct_del(&split);
			}
			vct_split(NULL, NULL, INIT);
		}
	}
	vct_del(&line);
	if (ret == FAILURE)
		exit_routine(ERR, strerror(errno));
}
