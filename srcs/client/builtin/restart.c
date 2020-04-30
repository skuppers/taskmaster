/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restart.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 16:01:38 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 18:15:11 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

/*
** ocp 0x01 : restart all
** ocp 0x02 : restart (<name> | <gname>:*) <...>
*/

t_vector	*blt_restart(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: restart requires a process name\n");
		help_restart();
		return (NULL);
	}
	else if (ft_strequ(cmd->av[0], "all") == TRUE)
	{
		cmd->ac = 1;
		return (generate_bytecode(cmd, 0x01));
	}
	return (generate_bytecode(cmd, 0x02));
}
