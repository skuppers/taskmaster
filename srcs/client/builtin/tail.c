/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tail.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:07:39 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 13:22:58 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static t_vector	*tail_option_f(t_cmd *cmd)
{
	int			i;

	if (cmd->ac == 1)
		ft_dprintf(STDERR_FILENO, "Error: tail requires process name\n");
	else if (cmd->ac > 3)
		ft_dprintf(STDERR_FILENO, "Error: too many arguments\n");
	else if (cmd->ac == 3 && ft_strequ(cmd->av[2], "stderr") == FALSE
			&& ft_strequ(cmd->av[2], "stdout") == FALSE)
		ft_dprintf(STDERR_FILENO, "Error: bad channel '%s'\n", cmd->av[2]);
	else
	{
		i = 0;
		while (i < cmd->ac)
		{
			cmd->av[i] = cmd->av[i + 1];
			i++;
		}
		if (cmd->ac == 2)
			cmd->av[1] = ft_strdup("stdout");
		else
			cmd->ac--;
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
		ft_printf("i = %d\n");
		cmd->av[i + 1] = cmd->av[i];
		i--;
	}
	cmd->av[0] = ft_strdup("-1600");
	cmd->ac++;
}

static t_vector	*tail_option_number(t_cmd *cmd)
{
	if (cmd->ac > 3)
		ft_dprintf(STDERR_FILENO, "Error: too many arguments\n");
	else if (cmd->ac == 1)
		ft_dprintf(STDERR_FILENO, "Error: too few arguments\n");
	else if (cmd->ac >= 3 && ft_strequ(cmd->av[2], "stderr") == FALSE
			&&  ft_strequ(cmd->av[2], "stdout") == FALSE)
		ft_dprintf(STDERR_FILENO, "Error: bad channel '%s'\n", cmd->av[2]);
	else
	{
		if (cmd->ac == 2)
		{
			cmd->av[2] = ft_strdup("stdout");
			cmd->ac++;
		}
		return (generate_bytecode(cmd, 0x02));
	}
	return (NULL);
}

t_vector		*blt_tail(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: too few arguments\n");
		return (NULL);
	}
	if (ft_strequ(cmd->av[0], "-f") == TRUE)
		return (tail_option_f(cmd));
	if (cmd->av[0][0] != '-')
		tail_get_dflt_nb(cmd);
	else if (ft_strcheck(cmd->av[0] + 1, ft_isdigit) == FALSE
			|| ft_strlen(cmd->av[0]) > 11 || ft_atol(cmd->av[0]) > INT_MAX)
	{
		ft_dprintf(STDERR_FILENO, "Error: bad argument %s\n", cmd->av[0]);
		return (NULL);
	}
	ft_memmove(cmd->av[0] , cmd->av[0] + 1, ft_strlen(cmd->av[0]));
	return (tail_option_number(cmd));
/*
** ocp 0x01 : tail -f <name> [stderr/stdout]
** ocp 0x02 : tail -NB <name> [stderr/stdout]
*/
}
