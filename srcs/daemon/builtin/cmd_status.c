/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_status.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:23:14 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 14:28:15 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_status(t_instance *instance, t_program *program)
{
	t_vector	*vct;
	char		*str;
	char		*state;
	t_vector	*uptime;

	vct = NULL;
	(void)program;
	
	if (instance != NULL && program != NULL)
	{
		state = get_instance_state(instance);
		if (instance->state == E_RUNNING)
		{
			uptime = vct_newstr(ctime(&instance->uptime));
			if (uptime != NULL && vct_len(uptime) > 6)
			{
				vct_cutfrom(uptime, vct_len(uptime) - 6);	
				if (uptime != NULL && vct_len(uptime) > 8)
					vct_popfrom(uptime, vct_len(uptime) - 8);
			}
			str = ft_asprintf("%s\t\t%s\tpid: %5d | uptime: %s\n", instance->name,
						state, instance->pid, vct_getstr(uptime));
			vct_del(&uptime);
		}
		else if (instance->state == E_STOPPED || instance->state == E_EXITED)
			str = ft_asprintf("%s\t\t%s\tstoptime: %s", instance->name,
						state, ctime(&instance->stop_time));
		else
			str = ft_asprintf("%s\t\t%s\n", instance->name, state);
			
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

