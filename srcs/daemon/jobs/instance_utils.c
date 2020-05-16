/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instance_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 13:08:43 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/16 10:14:53 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

int8_t		del_instance(t_program *prg, uint8_t id)
{
	t_instance	*before;
	t_instance	*to_del;
	t_instance	*after;

	to_del = get_instance(prg, id);
	if (to_del == NULL)
		return (ERR_UNDEF_INST);
	after = get_instance(prg, id + 1);
	if (id == 0)
	{
		prg->instance = after;
		if (to_del != NULL)
			free(to_del->name);
		free(to_del);
	}
	else
	{
		before = get_instance(prg, id - 1);
		before->next = after;
		if (to_del != NULL)
			free(to_del->name);
		free(to_del);
	}
	return (SUCCESS);
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

	if (instance->state == E_STARTING || instance->state == E_RUNNING
		|| instance->state == E_STOPPING)
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

void		try_to_kill_instance(t_program *prog, t_instance *instance)
{
	if (kill(instance->pid, SIGKILL) == FAILURE)
		tlog(E_LOGLVL_ERRO, "failed to kill instance %s: %s\n",
			instance->name, strerror(errno));
	else
	{
		tlog(E_LOGLVL_WARN, "killing %s after %d seconds\n",
					instance->name, prog->stopwaitsecs);
		reinit(instance, E_STOPPED, KEEP_BACKOFF, 0);
	}
}
