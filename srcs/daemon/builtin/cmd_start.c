/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_start.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:21:02 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 02:21:34 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_start(t_instance *instance, t_program *program)
{
	if (instance != NULL && program != NULL)
	{
		if (instance->state == E_RUNNING || instance->state == E_STARTING)
			return (get_msg(instance->name, "already started", ERR_MSG));
		if (start_instance(program, instance->id, g_env->environ) == SUCCESS)
			return (get_msg(instance->name, "started", INFO_MSG));
	}
	return (get_msg(instance->name, "start-up error", ERR_MSG));
}

t_vector			*cmd_start(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_start);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av, cmd->ac, action_start);
	return (vct);
}