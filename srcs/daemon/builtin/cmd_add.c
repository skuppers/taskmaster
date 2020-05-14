/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:28:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 15:37:31 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void		clean_node(void *d)
{
	((t_list *)d)->content = NULL;
}

t_vector	*action_add(t_instance *instance, t_program *program)
{
	uint8_t		inst_nb;
	int			launch_success;

	(void)instance;
	if (program->availmode != E_ADDED)
		return (get_msg(program->name, "already active", ERR_MSG));
	program->availmode = E_LOCKED;
	inst_nb = 0;
	program->pgid = 0;
	while (inst_nb < program->numprocs)
	{
		if (program->autostart == TRUE)
		{
			launch_success = start_instance(program, inst_nb, g_denv->environ);
			print_cmd_success("start", launch_success, program, inst_nb);
		}
		++inst_nb;
	}
	append_to_pgrmlist(g_denv, program);
	ft_lstdelnode(&g_tmpenv->prgm_list, program, clean_node);
	return (get_msg(program->name, "available", INFO_MSG));
}

t_vector	*cmd_add(t_cmd *cmd)
{
	return (exec_action_args_group(cmd->av, cmd->ac, action_add, g_tmpenv));
}
