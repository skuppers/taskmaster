/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 16:10:20 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 18:22:07 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static void	ask_help(t_cmd *cmd)
{
	static t_help	ft_help[] = {help_add, help_avail, help_clear, help_exit,
								help_fg, help_help, help_maintail, help_open,
								help_pid, help_quit, help_reload, help_remove,
								help_reread, help_restart, help_shutdown,
								help_signal, help_start, help_status, help_stop,
								help_tail, help_update, help_version};
	uint8_t			i;

	i = 0;
	while (i < NB_CMD)
	{
		if (ft_strequ(cmd->av[0], get_keyword(i)) == TRUE)
		{
			ft_help[i]();
			return ;
		}
		i++;
	}
	ft_dprintf(STDERR_FILENO, "*** No Help on %s\n", cmd->av[0]);
}

t_vector	*blt_help(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "\ndefault commands (type help <topic>):\n");
		ft_dprintf(STDERR_FILENO, "=====================================\n");
		ft_dprintf(STDERR_FILENO,
				"add\texit\t  open\treload\trestart\t  start\t  tail\n");
		ft_dprintf(STDERR_FILENO,
				"avail\tfg\t  pid\tremove\tshutdown  status  update\n");
		ft_dprintf(STDERR_FILENO,
				"clear\tmaintail  quit\treread\tsignal\t  stop\t  version\n\n");
	}
	else
		ask_help(cmd);
	return (NULL);
}
