/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_update_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 16:45:33 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/16 10:24:48 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_program	*get_modified_prog(t_program *old)
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

void		apply_changes(t_vector *msg)
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
			else
			{
				new->availmode = E_ADDED;
				vct_cat(msg, action_remove(NULL, old));
				vct_cat(msg, action_add(NULL, new));
			}
		}
		ptr = next;
	}
}

void		remove_progs(t_denv *env, t_vector *msg)
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

void		add_progs(t_denv *env, t_vector *msg)
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
