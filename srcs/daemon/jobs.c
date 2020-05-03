/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 18:16:00 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

int     child_process(t_program *prog, t_instance *instance)
{
//	FILE *stderrfile;
//	FILE *stdoutfile;

//	dprintf(STDERR_FILENO, "Prog %s pid %d pgid %d\n", prog->process_name, prog->pid, prog->pgid);

/*	stdoutfile = fopen( prog->stdout_logfile, "a+" );
	stderrfile = fopen( prog->stderr_logfile, "a+" );
    if (stdoutfile == NULL || stderrfile == NULL)
	{
        fputs( "Could not open file", stderr );
        exit (1);
    }
    dup2(fileno(stdoutfile), STDOUT_FILENO);
	dup2(fileno(stderrfile), STDERR_FILENO);
	fclose(stdoutfile);
	fclose(stderrfile);*/

    if (execve(prog->bin, prog->avs, NULL) == FAILURE)
		ft_dprintf(2, "taskmasterd: Program %s instance %d execution error\n",
				prog->name, instance->id);
	exit(1);
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

void		free_instance(t_instance *inst)
{
	free(inst);
}

int8_t		del_instance(t_program *prg, uint8_t id)
{
	t_instance	*before;
	t_instance	*to_del;
	t_instance	*after;

	to_del = get_instance(prg, id);
	if (to_del == NULL)
		return (-1);
	after = get_instance(prg, id + 1);
	if (id == 0)
	{
		prg->instance = after;
		free_instance(to_del);
	}
	else
	{
		before = get_instance(prg, id - 1);
		before->next = after;
		free_instance(to_del);
	}
	return (0);
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
			return (0);
		}
		ptr = ptr->next;
	}
	prg->instance = inst;
	return (0);
}

t_instance	*new_instance(uint8_t id)
{
	t_instance	*new;

	new = (t_instance*)malloc(sizeof(t_instance));
	if (new == NULL)
		return (NULL);
	new->id = id;
	new->state = E_STOPPED;
	new->pid = 0;
	new->start_time = 0;
	new->next = NULL;
	return (new);
}	

int8_t		start_instance(t_program *prog, uint8_t id)
{
	t_instance	*inst;

	inst = get_instance(prog, id);
	if (inst != NULL)
	{
		
		if ((inst->pid = fork()) < 0)
			return (-2);
		else if (inst->pid == 0)	// child 
			child_process(prog, inst);
		else
		{
			inst->state = E_STARTING;
			inst->start_time = time(NULL);
			return (0);		// launch success
		}
	}
	return (-1);	// No instance
}

int8_t	stop_instance(t_program *prog, uint8_t id)
{
	(void)prog;(void)id;
	// send SIGTERM signal to child
	// wait for <stopwaitsec> to return positive
	// if not stopped => send SIGKILL
	return (0);
}

char	*get_instance_state(t_instance *instance)
{
	if (instance->state == E_STARTING)
		return (STATE_STARTING);
	if (instance->state == E_RUNNING)
		return (STATE_RUNNING);
	if (instance->state == E_STOPPING)
		return (STATE_STOPPING);
	if (instance->state == E_STOPPED)
		return (STATE_STOPPED);
	return (STATE_UNKNOWN);
}

void    launch_jobs(t_env *env)
{
    t_list      *ptr;
    t_program   *prog;
	uint8_t		inst_nb;
	t_instance	*inst;
	int			launch_success;

    ptr = env->prgm_list;
    while (ptr != NULL)
    {
		inst_nb = 0;
        prog = ptr->content;
		while (inst_nb < prog->numprocs)
		{
			inst = new_instance(inst_nb);	// create instance meta
			if (inst == NULL)
			{
				dprintf(STDERR_FILENO, "Failed to allocate instance\n");
				break ;
			}
			add_instance(prog, inst);		//add instance to program_list
			if (prog->autostart == TRUE)
			{
				launch_success = start_instance(prog, inst_nb);
				if (launch_success == 0)
					dprintf(STDERR_FILENO, "Instance %d of %s with pid %d entered %s state.\n",
							inst_nb, prog->name, get_instance(prog, inst_nb)->pid,
							get_instance_state(get_instance(prog, inst_nb)));
				else
				{
					if (launch_success == -1)
						dprintf(STDERR_FILENO, "Failed to launch instance %d of %s: no instance.\n", inst_nb, prog->name);
					else if (launch_success == -2)
						dprintf(STDERR_FILENO, "Forking of instance %d from %s failed.\n", inst_nb, prog->name);
				}
			}
			++inst_nb;
		}
		ptr = ptr->next;
	}
}
