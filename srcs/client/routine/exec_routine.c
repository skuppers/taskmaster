/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 11:41:20 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 13:06:35 by ffoissey         ###   ########.fr       */
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

static int		send_and_receive(t_vector *trame)
{
	t_vector		*feedback;

	if (try_to_send_trame(g_env->unix_socket, trame,
							TO_PRINT, dprintf) == SUCCESS)
	{
		feedback = get_feedback();
		if (feedback == NULL)
		{
			dprintf(STDERR_FILENO, "Got no feedback from daemon!\n");
			return (FAILURE);
		}
		vct_print_fd(feedback, STDERR_FILENO);
		vct_del(&feedback);
		return (SUCCESS);
	}
	return (FAILURE);
}

int				routine(t_vector *line)
{
	t_vector		*trame;
	int				ret_value;

	g_env->cmd = get_cmd(line);
	trame = NULL;
	ret_value = FAILURE;
	if (g_env->cmd != NULL)
	{
		trame = get_trame(g_env->cmd);
		if (trame != NULL)
			ret_value = send_and_receive(trame);
	}
	vct_del(&trame);
	reset_cmd();
	return (ret_value);
}

static int		print_prompt(void)
{
	return (ft_dprintf(STDERR_FILENO, g_env->opt.str[PROMPT]));
}

void			read_cmd(void)
{
	t_vector	*line;
	int			ret;

	line = vct_new(DFL_VCT_SIZE);
	get_status();
	print_prompt();
	while ((ret = tsk_readline(line, STDIN_FILENO, g_env)) >= 0)
	{
		if (vct_apply(line, IS_SPACE) == false)
		{
			history(line, ADD | RESET);
			routine(line);
		}
		print_prompt();
	}
	vct_del(&line);
	if (ret == FAILURE)
		exit_routine(ERR, strerror(errno));
}
