/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_bytecode.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 11:47:07 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 11:30:16 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

static void	fill_cmd(t_vector *arg, t_cmd *cmd)
{
	char					**tab;
	int						i;

	if (arg != NULL)
	{
		cmd->av = ft_strsplit_whitespaces(vct_getstr(arg));
		cmd->ac = ft_tabsize(cmd->av);
	}
	tab = (char **)ft_memalloc(sizeof(char **) * (cmd->ac + 3));
	if (tab != NULL)
	{
		i = 0;
		while (i < cmd->ac)
		{
			tab[i] = cmd->av[i];
			i++;
		}
	}
	free(cmd->av);
	cmd->av = tab;
}

t_cmd		*get_cmd_struct(enum e_cmd_type type, t_vector *arg)
{
	static const uint8_t	flag[] = {TAB_ARG, NO_ARG, TAB_ARG, NO_ARG, TAB_ARG,
										ONE_ARG, TAB_ARG, ONE_ARG, TAB_ARG,
										NO_ARG, NO_ARG, TAB_ARG, NO_ARG,
										TAB_ARG, NO_ARG, TAB_ARG, TAB_ARG,
										TAB_ARG, TAB_ARG, TAB_ARG, TAB_ARG,
										NO_ARG, NO_ARG, NO_ARG};
	static t_cmd			cmd;

	bzero(&cmd, sizeof(t_cmd));
	cmd.type = type;
	if (flag[type] == ONE_ARG)
	{
		if (vct_apply(arg, IS_SPACE) == FALSE && vct_len(arg) != 0)
		{
			cmd.av = (char **)ft_memalloc(sizeof(char **) * 2);
			if (cmd.av != NULL)
			{
				cmd.av[0] = vct_dupstr(arg);
				cmd.ac = 1;
			}
		}
	}
	else if (flag[type] == TAB_ARG)
		fill_cmd(arg, &cmd);
	return (&cmd);
}

static void	add_size(t_vector *vct)
{
	uint32_t	size;

	size = (uint32_t)(vct_len(vct) + 4);
	vct_addcharat(vct, (char)((size >> 24) & 0xff), 1);
	vct_addcharat(vct, (char)((size >> 16) & 0xff), 1);
	vct_addcharat(vct, (char)((size >> 8) & 0xff), 1);
	vct_addcharat(vct, (char)(size & 0xff), 1);
}

t_vector	*generate_bytecode(t_cmd *cmd, int ocp)
{
	t_vector	*vct;
	int			i;

	vct = vct_new(DFL_VCT_SIZE);
	vct_add(vct, SOH);
	vct_add(vct, cmd->type + 128);
	vct_add(vct, ocp + 128);
	if (cmd->ac != 0)
		vct_add(vct, STX);
	i = 0;
	while (i < cmd->ac)
	{
		if (i != 0)
			vct_add(vct, US);
		vct_addstr(vct, cmd->av[i]);
		i++;
	}
	if (cmd->ac != 0)
		vct_add(vct, ETX);
	vct_add(vct, ENQ);
	add_size(vct);
	vct_add(vct, EOT);
	return (vct);
}

t_vector	*generate_feedback(t_vector *input)
{
	if (input != NULL)
	{
		vct_push(input, SOH);
		vct_add(input, ENQ);
		add_size(input);
		vct_add(input, EOT);
	}
	return (input);
}
