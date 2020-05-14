/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_restart.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:21:46 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 23:00:58 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_restart(t_instance *instance, t_program *program)
{
	enum e_prg_state	old_state;

	if (instance == NULL || program == NULL)
		return (NULL);
	old_state = instance->state;
	if (instance->state == E_RUNNING || instance->state == E_STARTING)
	{
		if (stop_instance(program, instance, program->stopsignal) == SUCCESS)
		{
			while (instance->state == old_state
					|| instance->state == E_STOPPING)
				waiter();
		}
	}
	old_state = instance->state;
	if (start_instance(program, instance->id, g_denv->environ) == SUCCESS)
	{
		while (instance->state == E_STARTING)
			waiter();
	}
	return (get_msg(instance->name, "restarted", INFO_MSG));
}

t_vector	*cmd_restart(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_restart);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av, cmd->ac, action_restart);
	return (vct);
}
