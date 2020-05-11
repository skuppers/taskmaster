/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 11:41:20 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/11 18:42:49 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static t_vector	*get_trame(t_cmd *cmd)
{
	static	t_builtin	builtin[] = {blt_add, blt_avail, blt_clear, blt_exit,
									blt_fg, blt_help, blt_maintail, blt_open,
									blt_pid, blt_quit, blt_reload, blt_remove,
									blt_reread, blt_restart, blt_shutdown,
									blt_signal, blt_start, blt_status, blt_stop,
									blt_tail, blt_update, blt_version};
	t_vector			*trame;

	trame = builtin[cmd->type](cmd);
	if (g_env->opt.mask & OPT_DEBUG && trame != NULL)
		debug_print_bytecode(trame);
	return (trame);
}

static t_vector		*send_and_receive(t_vector *trame, const uint8_t flag)
{
	t_vector		*feedback;

	feedback = NULL;
	if (try_to_send_trame(g_env->unix_socket, trame,
							TO_PRINT, ft_dprintf) == SUCCESS)
	{
		feedback = get_feedback();
		if (feedback == NULL && flag == DEL_FEEDBACK)
		{
			dprintf(STDERR_FILENO, "Got no feedback from daemon!\n");
			return (NULL);
		}
		if (flag == DEL_FEEDBACK)
			vct_print_fd(feedback, STDERR_FILENO);
	}
//	else error ? 
	return (feedback);
}

t_vector	*routine(t_vector *line, const uint8_t flag)
{
	t_vector		*trame;
	t_vector		*feedback;

	g_env->cmd = get_cmd(line);
	trame = NULL;
	feedback = NULL;
	if (g_env->cmd != NULL)
	{
		trame = get_trame(g_env->cmd);
		if (trame != NULL)
			feedback = send_and_receive(trame, flag);
	}
	vct_del(&trame);
	reset_cmd();
	if (flag == DEL_FEEDBACK)
		vct_del(&feedback);
	return (feedback);
}

void			read_cmd(void)
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
