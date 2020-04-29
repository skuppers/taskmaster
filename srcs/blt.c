/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 13:08:27 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 16:19:01 by ffoissey         ###   ########.fr       */
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
		cmd.av = ft_memalloc(sizeof(char **) * 2);
		cmd.av[0] = vct_dupstr(arg);
		cmd.ac = 1;
	}
	else if (flag[type] == TAB_ARG)
	{
		cmd.av = ft_strsplit_whitespaces(vct_getstr(arg));
		cmd.ac = get_tab_size(cmd.av);
	}
	return (&cmd);
}

t_cmd	*blt_add(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_avail(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_clear(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_exit(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_fg(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_maintail(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_open(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_pid(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_quit(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_reload(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_remove(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_reread(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_restart(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_shutdown(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_signal(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_start(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_status(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_stop(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_tail(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_update(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

t_cmd	*blt_version(t_vector *arg)
{
	(void)arg;
	return (NULL);
}

