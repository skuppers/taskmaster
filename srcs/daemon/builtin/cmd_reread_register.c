/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_reread_register.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 17:33:14 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 17:35:52 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void		activate_instances(t_program *prg)
{
	t_instance	*inst;
	uint8_t		inst_nb;

	inst_nb = 0;
	while (inst_nb < prg->numprocs)
	{
		inst = new_instance(inst_nb, prg->name);
		if (inst == NULL)
		{
			tlog(E_LOGLVL_ERRO, "Failed to allocate instance\n");
			break ;
		}
		add_instance(prg, inst);
		++inst_nb;
	}
}

void			register_added(t_denv *env, t_vector *v)
{
	t_list		*ptr;
	t_program	*prg;

	ptr = env->prgm_list;
	while (ptr != NULL)
	{
		prg = ptr->content;
		if (prg->availmode == E_ADDED)
		{
			activate_instances(prg);
			vct_addstr(v, prg->name);
			vct_addstr(v, " available\n");
		}
		ptr = ptr->next;
	}
}

void			register_modified(t_denv *env, t_vector *v)
{
	t_list		*ptr;
	t_program	*prg;

	ptr = env->prgm_list;
	while (ptr != NULL)
	{
		prg = ptr->content;
		if (prg->availmode == E_REMOVED)
		{
			vct_addstr(v, prg->name);
			vct_addstr(v, " disappeared\n");
		}
		else if (prg->availmode == E_CHANGED)
		{
			vct_addstr(v, prg->name);
			vct_addstr(v, " changed\n");
		}
		ptr = ptr->next;
	}
}

t_vector		*register_changes(t_denv *env)
{
	t_vector	*resp;

	resp = vct_newstr("");
	check_removed(env);
	check_added(env);
	check_modified(env);
	register_added(env, resp);
	register_modified(g_denv, resp);
	return (resp);
}
