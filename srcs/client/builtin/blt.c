/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 13:08:27 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 13:49:41 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

int		get_tab_size(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i] != NULL)
		i++;
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
	char					**tab;
	int						i;

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
		tab = ft_memalloc(sizeof(char **) * (cmd.ac + 3));
		i = 0;
		while (i < cmd.ac)
		{
			tab[i] = cmd.av[i];
			i++;
		}
		free(cmd.av);
		cmd.av = tab;
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
			vct_add(vct, ';');
		vct_addstr(vct, cmd->av[i]);
		i++;
	}
	vct_add(vct, (char)0xff);
	return (vct);
}

t_vector	*blt_help(t_cmd *cmd)                  // A FAIRE
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_restart(t_cmd *cmd)                // A FAIRE
{
	(void)cmd;
	return (NULL);
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
