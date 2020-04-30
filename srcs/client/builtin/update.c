/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:36:50 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 18:16:51 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

/*
** ocp 0x01 : update
** ocp 0x02 : update all
** ocp 0x03 : update <gname> <...>
*/

t_vector	*blt_update(t_cmd *cmd)
{
	if (cmd->ac == 0)
		return (generate_bytecode(cmd, 0x01));
	else if (ft_strequ(cmd->av[0], "all") == TRUE)
	{
		cmd->ac = 1;
		return (generate_bytecode(cmd, 0x02));
	}
	return (generate_bytecode(cmd, 0x03));
}
