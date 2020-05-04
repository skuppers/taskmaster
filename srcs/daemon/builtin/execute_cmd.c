/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 19:34:40 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector			*cmd_add(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_avail(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_clear(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_fg(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_maintail(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_open(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_pid(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_reload(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_remove(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_reread(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_restart(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_shutdown(t_cmd *cmd)
{
	(void)cmd;
	exit_routine();
	return (NULL);
}

t_vector			*cmd_signal(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_start(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_status(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_stop(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_tail(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_update(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_version(t_cmd *cmd)
{
	(void)cmd;
	t_vector *vct = vct_newstr("Version alpha 0.1\n");
	
	return (generate_feedback(vct));
}

static int			is_invalid_cmd(enum e_cmd_type type)
{
	return (type >= NB_CMD || type == HELP || type == EXIT || type == QUIT);
}

t_vector			*execute_cmd(t_cmd *cmd)
{
	static t_process_cmd	process_cmd[NB_CMD] = {cmd_add, cmd_avail, cmd_clear,
									NULL, cmd_fg, NULL, cmd_maintail, cmd_open,
									cmd_pid, NULL, cmd_reload, cmd_remove,
									cmd_reread, cmd_restart, cmd_shutdown,
									cmd_signal, cmd_start, cmd_status,
									cmd_stop, cmd_tail, cmd_update,
									cmd_version};

	if (is_invalid_cmd(cmd->type) == TRUE)	
	{
		ft_dprintf(STDERR_FILENO, "Cmd %#.2x is not recognized\n", cmd->type);
		return (NULL);
	}
	return (process_cmd[cmd->type](cmd));
}
