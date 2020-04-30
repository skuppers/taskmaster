/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 13:08:27 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 01:58:26 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

int		get_tab_size(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i] != NULL)
		i++;
//	if (i > ARG_MAX)
//		return (FAILURE); // ERREUR ARG_MAX
	return ((int)i);
}

t_cmd	*get_cmd_struct(enum e_cmd_type type, t_vector *arg)
{
	static const uint8_t	flag[] = {TAB_ARG, NO_ARG, TAB_ARG, NO_ARG, TAB_ARG,
										ONE_ARG, TAB_ARG, ONE_ARG, TAB_ARG,
										NO_ARG, NO_ARG, TAB_ARG, NO_ARG,
										TAB_ARG, NO_ARG, TAB_ARG, TAB_ARG,
										TAB_ARG, TAB_ARG, TAB_ARG, TAB_ARG,
										NO_ARG};
	static t_cmd			cmd;

	ft_bzero(&cmd, sizeof(t_cmd));
	cmd.type = type;
	if (flag[type] == ONE_ARG)
	{
		if (vct_apply(arg, IS_SPACE) == FALSE && vct_len(arg) != 0)
		{
			cmd.av = ft_memalloc(sizeof(char **) * 2);
			cmd.av[0] = vct_dupstr(arg);
			cmd.ac = 1;
		}
	}
	else if (flag[type] == TAB_ARG)
	{
		cmd.av = ft_strsplit_whitespaces(vct_getstr(arg));
		cmd.ac = get_tab_size(cmd.av);
	}
	return (&cmd);
}


t_vector	*generate_bytecode(t_cmd *cmd, int ocp)
{
	t_vector	*vct;
	int			i;

	vct = vct_new(DFL_VCT_SIZE);
	vct_add(vct, cmd->type);
	if (ocp != NO_OCP)
		vct_add(vct, ocp);
	i = 0;
	while (i < cmd->ac)
	{
		if (i != 0)
			vct_add(vct, ':');
		vct_addstr(vct, cmd->av[i]);
		i++;
	}
	vct_add(vct, (char)0xff);
	return (vct);
}

t_vector	*blt_add(t_cmd *cmd)
{
	if (cmd->ac != 0)
		return (generate_bytecode(cmd, NO_OCP));
	return (NULL);
}

t_vector	*blt_avail(t_cmd *cmd)
{
	return (generate_bytecode(cmd, NO_OCP));
}

t_vector	*blt_clear(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: clear requires a process name\n");
		// call "help clear"
		return (NULL);
	}
	else if (ft_strequ(cmd->av[0], "all") == TRUE)
	{
		cmd->ac = 1;
		return (generate_bytecode(cmd, 0x01));
	}
	return (generate_bytecode(cmd, 0x02));
/*
** ocp 0x01 : clear all
** ocp 0x02 : clear <name> <...>
*/
}

t_vector	*blt_exit(t_cmd *cmd)                          // A FAIRE
{
	/// call exit routine
	exit(0);
	(void)cmd;
	return (NULL);
}

t_vector	*blt_fg(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: no process name supplied\n");
		// call "help clear"
		return (NULL);
	}
	else if (cmd->ac > 1)
	{
		ft_dprintf(STDERR_FILENO, "Error: too many process names supplied\n");
		return (NULL);
	}
	return (generate_bytecode(cmd, NO_OCP));
}

t_vector	*blt_maintail(t_cmd *cmd)
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
	else if (cmd->ac == 0)
	{
		cmd->av = ft_memalloc(sizeof(char **) * 2);
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
	ft_memmove(cmd->av[0] , cmd->av[0] + 1, ft_strlen(cmd->av[0]));
	return (generate_bytecode(cmd, 0x02));
/*
** ocp 0x01 : maintail -f
** ocp 0x02 : maintail -NB
*/
}

t_vector	*blt_open(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: url must be http:// or unix://\n");
		return (NULL);
	}
	return (generate_bytecode(cmd, NO_OCP));
}

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

t_vector	*blt_help(t_cmd *cmd)                  // A FAIRE
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_quit(t_cmd *cmd)
{
	return (blt_exit(cmd));
}

t_vector	*blt_reload(t_cmd *cmd)
{
	// Really restart ? y/N
	return (generate_bytecode(cmd, NO_OCP));
}

t_vector	*blt_remove(t_cmd *cmd)
{
	if (cmd->ac == 0)
		return (NULL);
	return (generate_bytecode(cmd, NO_OCP));
}

t_vector	*blt_reread(t_cmd *cmd)
{
	return (generate_bytecode(cmd, NO_OCP));
}

t_vector	*blt_restart(t_cmd *cmd)                // A FAIRE
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_shutdown(t_cmd *cmd)
{
	// Really shutdown ? y/N
	return (generate_bytecode(cmd, NO_OCP));
}

t_vector	*blt_signal(t_cmd *cmd)                  // A FAIRE
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_start(t_cmd *cmd)                   // A FAIRE
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_status(t_cmd *cmd)                  // A FAIRE
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_stop(t_cmd *cmd)                    // A FAIRE
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_tail(t_cmd *cmd)                    // A FAIRE
{
	uint8_t	i;
	char	**tab;

	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: too few arguments\n");
		return (NULL);
	}
	else if (ft_strequ(cmd->av[0], "-f") == TRUE)
	{
		if (cmd->ac == 1)
		{
			ft_dprintf(STDERR_FILENO, "Error: tail requires process name\n");
			return (NULL);
		}
		else if (cmd->ac >= 3 && ft_strequ(cmd->av[2], "stderr") == FALSE
				&& ft_strequ(cmd->av[2], "stdout") == FALSE)
		{
			ft_dprintf(STDERR_FILENO, "Error: bad channel '%s'\n", cmd->av[2]);
			return (NULL);
		}
		i = 0;
		while (i < cmd->ac)
		{
			cmd->av[i] = cmd->av[i + 1];
			i++;
		}
		cmd->ac--;
		return (generate_bytecode(cmd, 0x01)); 
	}
	else if (cmd->av[0][0] != '-')
	{
		i = 0;
		tab = (char **)ft_memalloc(sizeof(char *) * (cmd->ac + 2));
		while (i < cmd->ac)
		{
			tab[i + 1] = cmd->av[i];
			i++;
		}
		cmd->ac++;
		tab[0] = ft_strdup("-1600");
		free(cmd->av);
		cmd->av = tab;
	}
	else if (ft_strcheck(cmd->av[0] + 1, ft_isdigit) == FALSE
			|| ft_strlen(cmd->av[0]) > 11 || ft_atol(cmd->av[0]) > INT_MAX)
	{
		ft_dprintf(STDERR_FILENO, "Error: bad argument %s\n", cmd->av[0]);
		return (NULL);
	}
	ft_memmove(cmd->av[0] , cmd->av[0] + 1, ft_strlen(cmd->av[0]));
	if (cmd->ac >= 3 && ft_strequ(cmd->av[2], "stderr") == FALSE
			&&  ft_strequ(cmd->av[2], "stdout") == FALSE)
	{
		ft_dprintf(STDERR_FILENO, "Error: bad channel '%s'\n", cmd->av[2]);
		return (NULL);
	}
	return (generate_bytecode(cmd, 0x02));
/*
** ocp 0x01 : tail -f <name> [stderr/stdout]
** ocp 0x02 : tail -NB <name> [stderr/stdout]
*/
}

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
/*
** ocp 0x01 : update
** ocp 0x02 : update all
** ocp 0x03 : update <gname> <...>
*/
}

t_vector	*blt_version(t_cmd *cmd)
{
	return (generate_bytecode(cmd, NO_OCP));
}

