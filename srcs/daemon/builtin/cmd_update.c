/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:33:41 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 15:14:29 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static t_program *get_modified_prog(t_program *old)
{
	t_list		*ptr;
	t_list		*next;
	t_program	*prg;

	ptr = g_tmpenv->prgm_list;
	while (ptr != NULL)
	{
		next = ptr->next;
		prg = ptr->content;
		if (ft_strequ(old->name, prg->name))
			return (prg);
		ptr = next;
	}
	return (NULL);
}

static void	apply_changes(t_vector *msg)
{
	t_list		*ptr;
	t_list		*next;
	t_program	*old;
	t_program	*new;

	ptr = g_denv->prgm_list;
	while (ptr != NULL)
	{
		next = ptr->next;
		old = ptr->content;
		if (old->availmode == E_CHANGED)
		{
			new = get_modified_prog(old);
			if (new == NULL)
				vct_addstr(msg, "Could not update prog.\n");
			new->availmode = E_ADDED;
			vct_cat(msg, action_remove(NULL, old));
			vct_cat(msg, action_add(NULL, new));
		}
		ptr = next;
	}
}

static void	remove_progs(t_denv *env, t_vector	*msg)
{
	t_list		*ptr;
	t_list		*next;
	t_program	*prg;

	ptr = env->prgm_list;
	while (ptr != NULL)
	{
		next = ptr->next;
		prg = ptr->content;
		if (prg->availmode == E_REMOVED)
			vct_cat(msg, action_remove(NULL, prg));
		ptr = next;
	}
}

static void	add_progs(t_denv *env, t_vector	*msg)
{
	t_list		*ptr;
	t_list		*next;
	t_program	*prg;

	ptr = env->prgm_list;
	while (ptr != NULL)
	{
		next = ptr->next;
		prg = ptr->content;
		if (prg->availmode == E_ADDED)
			vct_cat(msg, action_add(NULL, prg));
		ptr = next;
	}
}

t_vector	*action_update_all(t_instance *i, t_program *p)
{
	(void)i;(void)p;
	t_vector	*resp;

	resp = vct_newstr("");
	remove_progs(g_denv, resp);
	add_progs(g_tmpenv, resp);
	apply_changes(resp);
	return (resp);
}

t_vector		*action_update(char **av, int ac)
{
	t_program	*prg;
	t_vector	*vct;
	t_program	*new;

	(void)ac;
	prg = NULL;
	vct = vct_newstr("");
	dprintf(2, "Prog is :%s\n", av[0]);
	prg = find_program(av[0], g_denv);
	if (prg == NULL)
		prg = find_program(av[0], g_tmpenv);
	if (prg == NULL)
		return (get_msg(av[1], "not found", ERR_MSG));
	if (prg->availmode == E_ADDED)
	{
		return (action_add(NULL, prg));
	}
	else if (prg->availmode == E_REMOVED)
		return (action_remove(NULL, prg));
	else if (prg->availmode == E_CHANGED)
	{
		new = get_modified_prog(prg);
		if (new == NULL)
			vct_addstr(vct, "Could not update prog.\n");
		new->availmode = E_ADDED;
		vct_cat(vct, action_remove(NULL, prg));
		vct_cat(vct, action_add(NULL, new));
		return (vct);
	}
	else
		return (get_msg(av[0], "no changes", ERR_MSG));
}

t_vector			*cmd_update(t_cmd *cmd)
{
	if (cmd->ocp == 0x01 || cmd->ocp == 0x02)
		return (action_update_all(NULL, NULL));//exec_action_all_groups(action_update));
	return (action_update(cmd->av, cmd->ac));//exec_action_args_group(cmd->av, cmd->ac, action_update, g_denv));
}
