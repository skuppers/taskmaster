/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 13:08:27 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 17:22:02 by ffoissey         ###   ########.fr       */
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
	return (generate_bytecode(cmd, NO_OCP));
}

t_vector	*blt_avail(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_clear(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_exit(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_fg(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_maintail(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_open(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_pid(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_quit(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_reload(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_remove(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_reread(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_restart(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_shutdown(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_signal(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_start(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_status(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_stop(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_tail(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_update(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector	*blt_version(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

