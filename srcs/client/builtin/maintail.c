/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maintail.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:23:53 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 20:06:41 by ffoissey         ###   ########.fr       */
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
			|| ft_strcheck(cmd->av[0] + 1, ft_isdigit) == false
			|| ft_strlen(cmd->av[0]) > 11 || ft_atol(cmd->av[0]) > INT_MAX)
	{
		ft_dprintf(STDERR_FILENO, "Error: bad argument %s\n", cmd->av[0]);
		return (NULL);
	}
	g_env->flag_exec |= TAIL_NB;
	g_env->flag_exec |= (ft_atoi(cmd->av[0] + 1) << 8);
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
	if (ft_strequ(cmd->av[0], "-f") == true)
	{
		cmd->ac = 0;
		g_env->flag_exec |= TAIL_FIFO;
		return (generate_bytecode(cmd, 0x01));
	}
	return (maintail_option_number(cmd));
}
