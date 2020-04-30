/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:31:19 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 13:31:29 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_clear(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: clear requires a process name\n");
		// call "help clear"
		return (NULL);
	}
	else if (ft_strequ(cmd->av[0], "all") == TRUE)
	{
		cmd->ac = 1;
		return (generate_bytecode(cmd, 0x01));
	}
	return (generate_bytecode(cmd, 0x02));
/*
** ocp 0x01 : clear all
** ocp 0x02 : clear <name> <...>
*/
}
