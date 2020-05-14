/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_reread_checker.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 17:28:33 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 17:29:48 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static int8_t		check_inistring(dictionary *d)
{
	static char *inistr[] = {"", "user", "directory", "logfile", "loglevel",
							"childlogdir", "environment"};
	static char	base[30] = "taskmasterd:";
	char		*cur;
	int			i;

	i = 0;
	while (i < 8)
	{
		strcpy(base + 12, inistr[i]);
		cur = (i == 7) ? g_denv->opt.environ : g_denv->opt.str[i];
		if (ft_strequ(iniparser_getstring(d, base, NULL), cur) == false)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

int8_t				check_daemon_opts(dictionary *d)
{
	char	*tmp;
	bool	yn;
	bool	is_nodea;

	if (check_inistring(d) == FAILURE)
		return (FAILURE);
	tmp = (char *)iniparser_getstring(d, "taskmasterd:umask", NULL);
	if (tmp != NULL)
	{
		if (strtol(tmp, NULL, 8) != g_denv->opt.umask)
			return (FAILURE);
	}
	is_nodea = (bool)(g_denv->opt.optmask & OPT_NODAEMON);
	yn = get_nodaemon((char *)iniparser_getstring(d,
				"taskmasterd:nodaemon", NULL));
	if (yn != is_nodea)
		return (FAILURE);
	return (SUCCESS);
}

void				check_removed(t_denv *nenv)
{
	t_list		*orig_ptr;
	t_program	*orig_prg;
	t_list		*new_ptr;
	t_program	*new_prg;
	int			found;

	orig_ptr = g_denv->prgm_list;
	while (orig_ptr != NULL)
	{
		found = 0;
		orig_prg = orig_ptr->content;
		new_ptr = nenv->prgm_list;
		while (new_ptr != NULL && found == 0)
		{
			new_prg = new_ptr->content;
			if (ft_strequ(orig_prg->name, new_prg->name) == TRUE)
				found = 1;
			new_ptr = new_ptr->next;
		}
		if (found == 0)
			orig_prg->availmode = E_REMOVED;
		orig_ptr = orig_ptr->next;
	}
}

void				check_added(t_denv *nenv)
{
	t_list		*orig_ptr;
	t_program	*orig_prg;
	t_list		*new_ptr;
	t_program	*new_prg;
	int			found;

	new_ptr = nenv->prgm_list;
	while (new_ptr != NULL)
	{
		found = 0;
		new_prg = new_ptr->content;
		orig_ptr = g_denv->prgm_list;
		while (orig_ptr != NULL && found == 0)
		{
			orig_prg = orig_ptr->content;
			if (ft_strequ(new_prg->name, orig_prg->name) == TRUE)
				found = 1;
			orig_ptr = orig_ptr->next;
		}
		if (found == 0)
			new_prg->availmode = E_ADDED;
		new_ptr = new_ptr->next;
	}
}

void				check_modified(t_denv *nenv)
{
	t_list		*orig_ptr;
	t_program	*orig_prg;
	t_list		*new_ptr;
	t_program	*new_prg;

	orig_ptr = g_denv->prgm_list;
	while (orig_ptr != NULL)
	{
		orig_prg = orig_ptr->content;
		new_ptr = nenv->prgm_list;
		while (new_ptr != NULL)
		{
			new_prg = new_ptr->content;
			if (ft_strequ(orig_prg->name, new_prg->name) == TRUE)
				if (orig_prg->availmode == E_LOCKED
					&& compare_prog(orig_prg, new_prg) != 0)
					orig_prg->availmode = E_CHANGED;
			new_ptr = new_ptr->next;
		}
		orig_ptr = orig_ptr->next;
	}
}
