/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:33:41 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 16:55:11 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector		*action_update_all(t_instance *i, t_program *p)
{
	t_vector	*resp;

	(void)i;
	(void)p;
	if (g_tmpenv == NULL)
		return (get_msg("update", "no changes", INFO_MSG));
	resp = vct_newstr("");
	remove_progs(g_denv, resp);
	add_progs(g_tmpenv, resp);
	apply_changes(resp);
	return (resp);
}

t_vector		*action_changed(t_program *prg)
{
	t_program	*new;
	t_vector	*vct;
	t_vector	*tmp;

	new = get_modified_prog(prg);
	if (new == NULL)
		return (vct_newstr("Could not update prog.\n"));
	new->availmode = E_ADDED;
	vct = action_remove(NULL, prg);
	tmp = action_add(NULL, new);
	vct_cat(vct, tmp);
	vct_del(&tmp);
	return (vct);
}

t_vector		*action_update(char **av, int ac)
{
	t_program	*prg;

	(void)ac;
	prg = NULL;

	prg = find_program(av[0], g_denv);
	if (prg == NULL)
		prg = find_program(av[0], g_tmpenv);
	if (prg == NULL)
		return (get_msg(av[0], "not found", ERR_MSG));
	if (prg->availmode == E_ADDED)
		return (action_add(NULL, prg));
	else if (prg->availmode == E_REMOVED)
		return (action_remove(NULL, prg));
	else if (prg->availmode == E_CHANGED)
		return (action_changed(prg));
	return (get_msg(av[0], "no changes", ERR_MSG));
}

t_vector		*cmd_update(t_cmd *cmd)
{
	if (cmd->ocp == 0x01 || cmd->ocp == 0x02)
		return (action_update_all(NULL, NULL));
	return (action_update(cmd->av, cmd->ac));
}
