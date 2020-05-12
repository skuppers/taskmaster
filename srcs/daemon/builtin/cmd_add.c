/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:28:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 15:03:19 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_add(t_instance *instance, t_program *program)
{
	(void)instance;
	return (get_msg(program->name, "process group already active", ERR_MSG));
}

t_vector	*cmd_add(t_cmd *cmd)
{
	return (exec_action_args_group(cmd->av, cmd->ac, action_add));
}
