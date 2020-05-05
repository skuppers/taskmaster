/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:31:13 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 02:50:40 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_pid(t_instance *instance, t_program *program)
{
	t_vector *vct;
	char	*str;

	vct = NULL;
	if (instance != NULL && program != NULL)
	{
		str = ft_asprintf("%-15s\t%d\n", instance->name,
			(instance->state == E_RUNNING || instance->state == E_STARTING) ?
				instance->pid : 0);
		vct = vct_newstr(str);
		ft_strdel(&str);
		return (vct);
	}
	return (get_msg(instance->name, "Unknow error", ERR_MSG));
}

t_vector			*cmd_pid(t_cmd *cmd)
{
	t_vector	*vct;
	char		*str;

	vct = NULL;
	if (cmd->ocp == 0x01)
	{
		str = ft_asprintf("Taskmasterd    \t%d\n", getpid());
		vct = vct_newstr(str);
		ft_strdel(&str);
	}
	else if(cmd->ocp == 0x02)
		vct = exec_action_all(action_pid);
	else if (cmd->ocp == 0x03)
		vct = exec_action_args(cmd->av, cmd->ac, action_pid);
	return (vct);
}
