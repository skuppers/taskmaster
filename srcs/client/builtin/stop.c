/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 15:59:20 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 18:15:47 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

/*
** ocp 0x01 : stop all
** ocp 0x02 : stop (<name> | <gname>:*) <...>
*/

t_vector	*blt_stop(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: stop requires a process name\n");
		help_stop();
		return (NULL);
	}
	else if (ft_strequ(cmd->av[0], "all") == TRUE)
	{
		cmd->ac = 0;
		return (generate_bytecode(cmd, 0x01));
	}
	return (generate_bytecode(cmd, 0x02));
}
