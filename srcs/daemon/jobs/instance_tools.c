/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   instance_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/15 16:59:38 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_instance	*new_instance(uint8_t id, char *prog_name)
{
	t_instance	*new;

	new = (t_instance*)malloc(sizeof(t_instance));
	if (new == NULL)
		return (NULL);
	ft_memset(new, 0, sizeof(t_instance));
	new->id = id;
	new->name = ft_asprintf("%s:%d", prog_name, id);
	new->state = E_STOPPED;
	new->fd[0] = -1;
	new->fd[1] = -1;
	new->fd[2] = -1;
	new->next = NULL;
	return (new);
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

	if ((inst = get_instance(prog, id)) == NULL)
		return (ERR_UNDEF_INST);
	if (inst->state == E_STARTING)
		return (ERR_STARTING);
	if (inst->state == E_RUNNING)
		return (ERR_RUNNING);
	get_pipes(inst);
	if ((inst->pid = fork()) < 0)
		close_parrent_fd(inst);
	else if (inst->pid == 0)
		child_process(prog, inst, environ);
	else
	{
		if (prog->pgid == 0)
			prog->pgid = inst->pid;
		inst->state = E_STARTING;
		inst->start_time = time(NULL);
	}
	close_child_fd(inst);
	return (inst->pid < 0 ? ERR_FORK : SUCCESS);
}

int8_t		stop_instance(t_program *prog, t_instance *instance, int signo)
{
	(void)prog;
	if (instance->state == E_STOPPING)
		return (ERR_STOPPING);
	if (instance->state == E_STOPPED)
		return (ERR_STOPPED);
	if (instance->state == E_EXITED)
		return (ERR_EXITED);
	if (instance->state == E_FATAL)
		return (ERR_FATAL);
	if (kill(instance->pid, signo) == FAILURE)
	{
		tlog(E_LOGLVL_ERRO, "failed to terminate instance %s: %s\n",
			instance->name, strerror(errno));
		return (FAILURE);
	}
	instance->stop_time = time(NULL);
	instance->state = E_STOPPING;
	return (SUCCESS);
}

void		stop_prog(t_program *program)
{
	t_instance			*ptr;
	t_instance			*next;

	ptr = program->instance;
	while (ptr != NULL)
	{
		next = ptr->next;
		if (stop_instance(program, ptr, program->stopsignal) == SUCCESS)
			while (ptr->state == E_STOPPING)
				waiter();
		del_instance(program, 0);
		ptr = next;
	}
}
