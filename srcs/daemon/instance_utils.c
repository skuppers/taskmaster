/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instance_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 18:46:18 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

char	*get_instance_state(t_instance *instance)
{
	if (instance->state == E_STARTING)
		return (STATE_STARTING);
	if (instance->state == E_BACKOFF)
		return (STATE_BACKOFF);
	if (instance->state == E_RUNNING)
		return (STATE_RUNNING);
	if (instance->state == E_STOPPING)
		return (STATE_STOPPING);
	if (instance->state == E_STOPPED)
		return (STATE_STOPPED);
	if (instance->state == E_EXITED)
		return (STATE_EXITED);
	if (instance->state == E_FATAL)
		return (STATE_FATAL);
	return (STATE_UNKNOWN);
}

t_instance	*get_instance(t_program *prg, uint8_t id)
{
	t_instance *ptr;

	ptr = prg->instance;
	while (ptr != NULL)
	{
		if (ptr->id == id)
			return (ptr);
		ptr = ptr->next;
	}
	return (NULL);
}

void		update_instance_uptime(t_instance *instance)
{
	time_t	now;

	if (instance->state != E_STOPPED)
	{
		now = time(NULL);
		instance->uptime = (uint32_t)(now - instance->start_time);
	}
}

int8_t		is_expected_exitcode(t_program *prg, t_instance *inst)
{
	int i;

	i = 0;
	while (prg->exitcodes[i])
	{
		if (ft_atoi(prg->exitcodes[i]) == inst->exitcode)
			return (TRUE);
		++i;
	}
	return (FALSE);
}