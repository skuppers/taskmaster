/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_remove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:32:31 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 15:12:01 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_remove(t_instance *instance, t_program *program)
{
	(void)instance;	
	return (get_msg(program->name, "process group still running", ERR_MSG));
}

t_vector			*cmd_remove(t_cmd *cmd)
{
	return (exec_action_args_group(cmd->av, cmd->ac, action_remove));
}
