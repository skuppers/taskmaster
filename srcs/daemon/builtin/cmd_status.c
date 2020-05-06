/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_status.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:23:14 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/06 17:15:58 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_status(t_instance *instance, t_program *program)
{
	t_vector	*vct;
	char		*str;
	char		*state;
	struct tm	*timer;

	vct = NULL;
	str = NULL;
	(void)program;
	if (instance != NULL && program != NULL)
	{
		state = get_instance_state(instance);
		if (instance->state == E_RUNNING)
		{
			timer = gmtime(&instance->uptime);
			if (timer != NULL)
			{
				if (instance->uptime < 3600)
					timer->tm_hour = 0;
				str = ft_asprintf("%-20s%-8spid: %5d | [uptime]\t% 9.2u:%.2u:%.2u\n",
						instance->name, state, instance->pid, timer->tm_hour,
						timer->tm_min, timer->tm_sec);
			}
		}
		else if (instance->state == E_STOPPED || instance->state == E_EXITED)
			str = ft_asprintf("%-20s%-18s | [stoptime] %s", instance->name,
						state, ctime(&instance->stop_time));
		else
			str = ft_asprintf("%-20s%s\n", instance->name, state);
			
		vct = vct_newstr(str);
		ft_strdel(&str);
	}
	return (vct);
}

t_vector			*cmd_status(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_status);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av, cmd->ac, action_status);
	return (vct);
}

