/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maintail.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:23:53 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 19:59:00 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static t_vector	*maintail_option_number(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		cmd->av[0] = ft_strdup("-1600");
		cmd->ac = 1;
	}
	else if (cmd->av[0][0] != '-'
			|| ft_strcheck(cmd->av[0] + 1, ft_isdigit) == FALSE
			|| ft_strlen(cmd->av[0]) > 11 || ft_atol(cmd->av[0]) > INT_MAX)
	{
		ft_dprintf(STDERR_FILENO, "Error: bad argument %s\n", cmd->av[0]);
		return (NULL);
	}
	ft_memmove(cmd->av[0], cmd->av[0] + 1, ft_strlen(cmd->av[0]));
	return (generate_bytecode(cmd, 0x02));
}

/*
** ocp 0x01 : maintail -f
** ocp 0x02 : maintail -NB
*/

t_vector		*blt_maintail(t_cmd *cmd)
{
	if (cmd->ac > 1)
	{
		ft_dprintf(STDERR_FILENO, "Error: too many arguments\n");
		return (NULL);
	}
	else if (ft_strequ(cmd->av[0], "-f") == TRUE)
	{
		cmd->ac = 0;
		return (generate_bytecode(cmd, 0x01));
	}
	return (maintail_option_number(cmd));
}
