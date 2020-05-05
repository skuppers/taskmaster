/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_stop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:22:53 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 02:23:07 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_stop(t_instance *instance, t_program *program)
{
	if (instance != NULL && program != NULL)
	{
		if (instance->state != E_RUNNING && instance->state != E_STARTING)
			return (get_msg(instance->name, "already stoped", ERR_MSG));
		if (stop_instance(program, instance, program->stopsignal) == SUCCESS)
			return (get_msg(instance->name, "stoped", INFO_MSG));
	}
	return (get_msg(instance->name, "unknow error", ERR_MSG));
}

t_vector			*cmd_stop(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_stop);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av, cmd->ac, action_stop);
	return (vct);
}
