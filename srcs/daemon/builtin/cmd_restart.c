/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_restart.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:21:46 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 23:39:41 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_restart(t_instance *instance, t_program *program)
{
	if (instance == NULL || program == NULL)
		return (NULL);
	if (instance->state == E_RUNNING || instance->state == E_STARTING)
	{
		if (stop_instance(program, instance, program->stopsignal) == SUCCESS)
			while (instance->state == E_STOPPING)
				instance_waiter(program, instance);
	}
	if (start_instance(program, instance->id, g_denv->environ) == SUCCESS)
	{
		log_state_information(instance);
		while (instance->state == E_STARTING)
			instance_waiter(program, instance);
		if (instance->state == E_FATAL || instance->state == E_BACKOFF)
			return (get_msg(instance->name, "spawn error", ERR_MSG));
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
