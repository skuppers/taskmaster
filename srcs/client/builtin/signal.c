/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 16:02:13 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 16:09:37 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_signal(t_cmd *cmd)
{
	if (cmd->ac < 2)
	{
		ft_dprintf(STDERR_FILENO,
			"Error: signal requires a signal name and a process name\n");
		// call "help"
		return (NULL);
	}
	else if (ft_strequ(cmd->av[1], "all") == TRUE)
	{
		cmd->ac = 1;
		return (generate_bytecode(cmd, 0x01));
	}
	return (generate_bytecode(cmd, 0x02));
/*
** ocp 0x01 : signal <signal name> all
** ocp 0x02 : signal <signal name> (<name> | <gname>:*) <...>
*/
}
