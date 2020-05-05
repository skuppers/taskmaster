/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 12:49:15 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 21:07:44 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static enum e_cmd_type	get_cmd_type(t_vector *word)
{
	uint8_t	i;

	i = 0;
	while (i < NB_CMD)
	{
		if (ft_strequ(vct_getstr(word), get_keyword(i)) == TRUE)
			return ((enum e_cmd_type)i);
		i++;
	}
	return (BAD_CMD);
}

static t_vector	*clean_line(t_vector *line)
{
	vct_replacechar(line, '\n', ' ');
	vct_replacechar(line, '\t', ' ');
	vct_trimfront(line, " ");
	return (line);
}

t_cmd	*get_cmd(t_vector *line)
{
	t_cmd				*cmd;
	t_vector			*cmd_string;
	enum e_cmd_type		cmd_type;

	cmd = NULL;
	cmd_string = vct_splitchr(clean_line(line), ' ', DEL_CHAR);
	cmd_type = get_cmd_type(cmd_string);
	if (cmd_type == BAD_CMD)
		ft_dprintf(STDERR_FILENO, "*** Unknown syntax: %s %s\n",
					vct_getstr(cmd_string), vct_getstr(line));
	else
	{
		vct_trimfront(line, " ");
		cmd = get_cmd_struct(cmd_type, line);
	}
	vct_del(&cmd_string);
//	debug_cmd(cmd); // DEBUG
	return (cmd);
}

t_vector	*get_trame(t_cmd *cmd)
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
		debug_print_bytecode(trame); // DEBUG TRAME
	return (trame);	
}

void	reset_cmd(void)
{
	if (g_env->cmd != NULL)
	{
		ft_free_tab_str(g_env->cmd->av);
		g_env->cmd->av = NULL;
	}
	g_env->cmd = NULL;
}

int	send_and_receive(t_vector *trame)
{
	t_vector		*feedback;
	
	if (try_to_send_trame(g_env->unix_socket, trame, TO_PRINT, dprintf) == SUCCESS)
	{
		feedback = get_feedback(g_env);
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

int	routine(t_vector *line)
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
