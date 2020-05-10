/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 16:01:05 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 13:08:02 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

/*
** ocp 0x01 : status [all]
** ocp 0x02 : start (<name> | <gname>:*) <...>
*/

t_vector	*blt_status(t_cmd *cmd)
{
	if (cmd->ac == 0 || ft_strequ(cmd->av[0], "all") == true)
	{
		cmd->ac = 0;
		return (generate_bytecode(cmd, 0x01));
	}
	return (generate_bytecode(cmd, 0x02));
}
