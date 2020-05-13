/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/13 13:01:03 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector			*get_logfile_name(t_program *program, t_instance *instance,
							const int fd, const char c)
{
	t_vector	*log;
	char		*id_str;

	if (g_denv->opt.str[CHILDLOGDIR] == NULL || fd <= 0 || fd > STDERR_FILENO
			|| program == NULL || instance == NULL || program->name == NULL)
		return (get_msg(instance->name, "no log file", ERR_MSG));
	log = vct_newstr(g_denv->opt.str[CHILDLOGDIR]);
	if (vct_getlastchar(log) != '/')
		vct_add(log, '/');
	vct_addstr(log, program->name);
	vct_add(log, '_');
	id_str = ft_itoa(instance->id);
	if (id_str != NULL)
		vct_addstr(log, id_str);
	ft_strdel(&id_str);
	vct_add(log, '_');
	vct_addstr(log, (fd == STDERR_FILENO) ? "err.log" : "out.log");
	if (c == ETX)
		vct_add(log, ETX);
	return (log);
}

static int			is_invalid_cmd(enum e_cmd_type type)
{
	return (type >= NB_CMD || type == HELP || type == EXIT || type == QUIT);
}

t_vector			*execute_cmd(t_cmd *cmd)
{
	static t_process_cmd	process_cmd[NB_CMD] = {cmd_add, cmd_avail, cmd_clear,
									NULL, cmd_fg, NULL, cmd_maintail, NULL,
									cmd_pid, NULL, cmd_reload, cmd_remove,
									cmd_reread, cmd_restart, cmd_shutdown,
									cmd_signal, cmd_start, cmd_status,
									cmd_stop, cmd_tail, cmd_update,
									cmd_version};

	if (is_invalid_cmd(cmd->type) == TRUE)	
	{
		tlog(E_LOGLVL_INFO, "Cmd %#.2x is not recognized\n", cmd->type);
		return (NULL);
	}
	return (generate_feedback(process_cmd[cmd->type](cmd)));
}
