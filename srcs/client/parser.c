/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 12:08:50 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static enum e_cmd_type	get_cmd(t_vector *word)
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

int	parser(t_vector *line)
{
	t_vector			*cmd_string;
	t_vector			*bytecode;
	enum e_cmd_type		cmd_type;
	static	t_builtin	builtin[] = {blt_add, blt_avail, blt_clear, blt_exit,
									blt_fg, blt_help, blt_maintail, blt_open,
									blt_pid, blt_quit, blt_reload, blt_remove,
									blt_reread, blt_restart, blt_shutdown,
									blt_signal, blt_start, blt_status, blt_stop,
									blt_tail, blt_update, blt_version};

	cmd_string = vct_splitchr(clean_line(line), ' ', DEL_CHAR);
	cmd_type = get_cmd(cmd_string);
	if (cmd_type == BAD_CMD)
	{
		ft_dprintf(STDERR_FILENO, "*** Unknown syntax: %s %s\n",
					vct_getstr(cmd_string), vct_getstr(line));
		vct_del(&cmd_string);
		return (FAILURE);
	}
	vct_trimfront(line, " ");
	g_env->cmd = get_cmd_struct(cmd_type, line);
//	debug_cmd(g_env->cmd); // DEBUG
	bytecode = builtin[cmd_type](g_env->cmd);
	if (bytecode != NULL)
	{
		if (g_env->opt.mask & OPT_DEBUG)
			debug_print_bytecode(bytecode);	
			///////////////// LAUNCH CMD
			send_bytecode(bytecode, vct_len(bytecode));
	//	request_daemon(bytecode, vct_len(bytecode));
			
	}
	ft_free_tab_str(g_env->cmd->av);
	g_env->cmd->av = NULL;
	vct_del(&bytecode);
	vct_del(&cmd_string);
	return (SUCCESS);
}
