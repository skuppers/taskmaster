/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tail.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:07:39 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 11:17:39 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static void		cmd_change(t_cmd *cmd)
{
	int			i;

	i = 0;
	free(cmd->av[i]);
	while (i < cmd->ac)
	{
		cmd->av[i] = cmd->av[i + 1];
		i++;
	}
	if (cmd->ac == 2)
		cmd->av[1] = strdup("stdout");
	else
		cmd->ac--;
}

static t_vector	*tail_option_f(t_cmd *cmd)
{
	if (cmd->ac == 1)
		dprintf(STDERR_FILENO, "Error: tail requires process name\n");
	else if (cmd->ac > 3)
		dprintf(STDERR_FILENO, "Error: too many arguments\n");
	else if (cmd->ac == 3 && ft_strequ(cmd->av[2], "stderr") == false
			&& ft_strequ(cmd->av[2], "stdout") == false)
		dprintf(STDERR_FILENO, "Error: bad channel '%s'\n", cmd->av[2]);
	else
	{
		cmd_change(cmd);
		g_env->flag_exec |= TAIL_FIFO;
		return (generate_bytecode(cmd, 0x01));
	}
	return (NULL);
}

static void		tail_get_dflt_nb(t_cmd *cmd)
{
	int		i;

	i = cmd->ac - 1;
	while (i >= 0)
	{
		cmd->av[i + 1] = cmd->av[i];
		i--;
	}
	cmd->av[0] = strdup("-1600");
	cmd->ac++;
}

static t_vector	*tail_option_number(t_cmd *cmd)
{
	if (cmd->ac > 3)
		dprintf(STDERR_FILENO, "Error: too many arguments\n");
	else if (cmd->ac == 1)
		dprintf(STDERR_FILENO, "Error: too few arguments\n");
	else if (cmd->ac >= 3 && ft_strequ(cmd->av[2], "stderr") == false
			&& ft_strequ(cmd->av[2], "stdout") == false)
		dprintf(STDERR_FILENO, "Error: bad channel '%s'\n", cmd->av[2]);
	else
	{
		if (cmd->ac == 2)
		{
			cmd->av[2] = strdup("stdout");
			cmd->ac++;
		}
		g_env->flag_exec |= TAIL_NB;
		g_env->flag_exec |= (atol(cmd->av[0]) << 8);
		return (generate_bytecode(cmd, 0x02));
	}
	return (NULL);
}

/*
** ocp 0x01 : tail -f <name> [stderr/stdout]
** ocp 0x02 : tail -NB <name> [stderr/stdout]
*/

t_vector		*blt_tail(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		dprintf(STDERR_FILENO, "Error: too few arguments\n");
		return (NULL);
	}
	if (ft_strequ(cmd->av[0], "-f") == true)
		return (tail_option_f(cmd));
	if (cmd->av[0][0] != '-')
		tail_get_dflt_nb(cmd);
	else if (ft_strcheck(cmd->av[0] + 1, isdigit) == false
			|| strlen(cmd->av[0]) > 10 || atol(cmd->av[0] + 1) > INT_MAX)
	{
		dprintf(STDERR_FILENO, "Error: bad argument %s\n", cmd->av[0]);
		return (NULL);
	}
	memmove(cmd->av[0], cmd->av[0] + 1, strlen(cmd->av[0]));
	return (tail_option_number(cmd));
}
