/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 16:02:13 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 11:13:12 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

/*
** ocp 0x01 : signal <signal name> all
** ocp 0x02 : signal <signal name> (<name> | <gname>:*) <...>
*/

t_vector	*blt_signal(t_cmd *cmd)
{
	if (cmd->ac < 2)
	{
		dprintf(STDERR_FILENO,
			"Error: signal requires a signal name and a process name\n");
		help_signal();
		return (NULL);
	}
	else if (ft_strequ(cmd->av[1], "all") == true)
	{
		cmd->ac = 1;
		return (generate_bytecode(cmd, 0x01));
	}
	return (generate_bytecode(cmd, 0x02));
}
