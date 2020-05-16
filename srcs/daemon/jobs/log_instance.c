/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_instance.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuppers <skuppers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/16 10:14:01 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

char		*get_instance_state(t_instance *instance)
{
	static char	*str_instance[] = {STATE_STARTING, STATE_BACKOFF, STATE_RUNNING,
									STATE_STOPPING, STATE_STOPPED, STATE_EXITED,
									STATE_FATAL, STATE_UNKNOWN};

	return (str_instance[instance->state]);
}

void		log_state_information(t_instance *instance)
{
	if (instance->state == E_EXITED)
		tlog(E_LOGLVL_INFO, "==> Instance %s exited with code %d\n",
			instance->name, instance->exitcode);
	else if (instance->state == E_STARTING)
		tlog(E_LOGLVL_INFO, "Instance %s with pid %d entered %s state.\n",
				instance->name, instance->pid, get_instance_state(instance));
	else if (instance->state == E_BACKOFF)
		tlog(E_LOGLVL_INFO, "Instance %s entered %s state.\n",
				instance->name, get_instance_state(instance));
	else if (instance->state == E_STOPPED)
		tlog(E_LOGLVL_INFO, "Instance %s %s by forced exit\n",
				instance->name, get_instance_state(instance));
	else if (instance->state == E_STOPPING)
		tlog(E_LOGLVL_INFO, "Instance %s entered %s state.\n",
				instance->name, get_instance_state(instance));
	else if (instance->state == E_RUNNING)
		tlog(E_LOGLVL_INFO,
				"Instance %s entered %s state after %d secs\n",
				instance->name, STATE_RUNNING, instance->uptime);
	else if (instance->state == E_FATAL)
		tlog(E_LOGLVL_INFO,
				"=====> Instance %s entered FATAL state.\n", instance->name);
}
