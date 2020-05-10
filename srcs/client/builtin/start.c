/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 16:00:21 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 13:07:33 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

/*
** ocp 0x01 : start all
** ocp 0x02 : start (<name> | <gname>:*) <...>
*/

t_vector	*blt_start(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: start requires a process name\n");
		help_start();
		return (NULL);
	}
	else if (ft_strequ(cmd->av[0], "all") == true)
	{
		cmd->ac = 0;
		return (generate_bytecode(cmd, 0x01));
	}
	return (generate_bytecode(cmd, 0x02));
}
