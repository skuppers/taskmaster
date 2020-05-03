/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instance_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 18:46:18 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_instance	*new_instance(uint8_t id)
{
	t_instance	*new;

	new = (t_instance*)malloc(sizeof(t_instance));
	if (new == NULL)
		return (NULL);
    ft_memset(new, 0, sizeof(t_instance));
	new->id = id;
	new->state = E_STOPPED;
	new->next = NULL;
	return (new);
}	

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
		free(to_del);
	}
	else
	{
		before = get_instance(prg, id - 1);
		before->next = after;
		free(to_del);
	}
	return (SUCCESS);
}

int8_t		add_instance(t_program *prg, t_instance *inst)
{
	t_instance *ptr;

	ptr = prg->instance;
	while (ptr != NULL)
	{
		if (ptr->next == NULL)
		{
			ptr->next = inst;
			return (SUCCESS);
		}
		ptr = ptr->next;
	}
	prg->instance = inst;
	return (SUCCESS);
}

int8_t		start_instance(t_program *prog, uint8_t id, t_list *environ)
{
	t_instance	*inst;

	inst = get_instance(prog, id);
	if (inst == NULL)
		return (ERR_UNDEF_INST);
	if (inst->state == E_STARTING)
		return (ERR_STARTING);
	if (inst->state == E_RUNNING)
		return (ERR_RUNNING);
	if ((inst->pid = fork()) < 0)
		return (ERR_FORK);
	else if (inst->pid == 0)	// child 
		child_process(prog, inst, environ);
	inst->state = E_STARTING;
	inst->start_time = time(NULL);
	return (SUCCESS);		// launch success
}

int8_t		stop_instance(t_program *prog, t_instance *instance)
{
	if (instance->state == E_STOPPING)
		return (ERR_STOPPING);
	if (instance->state == E_STOPPED)
		return (ERR_STOPPED);
	kill(instance->pid, prog->stopsignal);
	instance->stop_time = time(NULL);
	instance->state = E_STOPPING;
	return (SUCCESS);
}
