/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 02:34:00 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

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
		ft_dprintf(STDERR_FILENO, "Cmd %#.2x is not recognized\n", cmd->type);
		return (NULL);
	}
	return (generate_feedback(process_cmd[cmd->type](cmd)));
}
