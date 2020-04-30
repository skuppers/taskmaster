/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:34:09 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 13:34:26 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_pid(t_cmd *cmd)
{
	if (cmd->ac == 0)
		return (generate_bytecode(cmd, 0x01));
	else if (ft_strequ(cmd->av[0], "all") == TRUE)
	{
		cmd->ac = 1;
		return (generate_bytecode(cmd, 0x02));
	}
	return (generate_bytecode(cmd, 0x03));
/*
** ocp 0x01 : pid
** ocp 0x02 : pid all
** ocp 0x03 : pid <name> <...>
*/
}
