/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maintail.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:23:53 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 11:16:28 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static t_vector	*maintail_option_number(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		cmd->av[0] = strdup("-1600");
		cmd->ac = 1;
	}
	else if (cmd->av[0][0] != '-'
			|| ft_strcheck(cmd->av[0] + 1, isdigit) == false
			|| strlen(cmd->av[0]) > 11 || atol(cmd->av[0]) > INT_MAX)
	{
		dprintf(STDERR_FILENO, "Error: bad argument %s\n", cmd->av[0]);
		return (NULL);
	}
	g_env->flag_exec |= TAIL_NB;
	g_env->flag_exec |= (atol(cmd->av[0] + 1) << 8);
	memmove(cmd->av[0], cmd->av[0] + 1, strlen(cmd->av[0]));
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
		dprintf(STDERR_FILENO, "Error: too many arguments\n");
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
