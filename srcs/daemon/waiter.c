/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/04 20:23:25 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void    instance_stopped(t_program *prog, t_instance *instance)
{
    instance->state = E_STOPPED;
    dprintf(STDERR_FILENO, "Instance %d of %s entered %s state.\n",
                instance->id, prog->name, STATE_STOPPED); // STATE_PAUSED ?
}

void    instance_continued(t_program *prog, t_instance *instance)
{
    instance->state = E_RUNNING;
    dprintf(STDERR_FILENO, "Instance %d of %s entered %s state.\n",
                instance->id, prog->name, STATE_RUNNING);
}

void		check_instance(t_program *prog, t_instance *instance)
{
	if (instance->state == E_STARTING && instance->uptime >= prog->startsecs)
	{
		instance->state = E_RUNNING;
   		dprintf(STDERR_FILENO, "Instance %d of %s entered %s state after %d secs\n",
                instance->id, prog->name, STATE_RUNNING, instance->uptime);
	}
	else if (instance->state == E_STOPPING
		&& instance->uptime >= (instance->stop_time + prog->stopwaitsecs))
	{
		kill(instance->pid, SIGKILL); //TODO protect
		instance->state = E_STOPPED;
		instance->pid = 0;
		instance->start_time = 0;
		instance->stop_time = time(NULL);
		dprintf(STDERR_FILENO, "Instance %d of %s %s by forced exit\n",
                instance->id, prog->name, get_instance_state(instance));
	}
	else if (instance->state == E_BACKOFF)
	{
		start_instance(prog, instance->id, g_env->environ);
	//	print_cmd_success("(re)start", ret, prog, instance->id);
		dprintf(STDERR_FILENO, "Instance %d of %s with pid %d entered %s state.\n",
							instance->id, prog->name,instance->pid,
							get_instance_state(instance));
	}
	else if (instance->state == E_EXITED && prog->autorestart != FALSE)
	{
		if (prog->autorestart == TRUE
			|| (prog->autorestart == UNEXPECTED && !is_expected_exitcode(prog, instance))) // always
		{
			start_instance(prog, instance->id, g_env->environ);
			dprintf(STDERR_FILENO, "Instance %d of %s with pid %d entered %s state.\n",
							instance->id, prog->name,instance->pid,
							get_instance_state(instance));
			//print_cmd_success("(re)start", ret, prog, instance->id);
		}
	}
}

void        terminate_instance(t_program *prog, t_instance *instance, int status)
{
	if (WIFSTOPPED(status))
		instance_stopped(prog, instance); // Ecriture sur terminal ? tcsetpgrp()
	else if (WIFCONTINUED(status))
		instance_continued(prog, instance);
	else
	{
		if (instance->uptime < prog->startsecs) // Crash avant running state
		{
			if (instance->backoff >= prog->startretries)
			{
				dprintf(STDERR_FILENO, "=====> Instance %d of %s entered FATAL state.\n",
					instance->id, prog->name);
				instance->state = E_FATAL;
				instance->backoff = 0;
			}
			else
			{
				dprintf(STDERR_FILENO, "===> Instance %d of %s backoff.\n",
				instance->id, prog->name);
				instance->state = E_BACKOFF;
				instance->backoff++;
			}
			instance->pid = 0;
			instance->start_time = 0;
			instance->stop_time = time(NULL);
			instance->uptime = 0;
			instance->exitcode = 0;
		}
		else // was in RUNNING state
		{
			if (instance->state == E_STOPPING) // stopped by user dont restart
			{	// SIGCHLD proper way
				instance->state = E_STOPPED;
				instance->pid = 0;
				instance->start_time = 0;
				instance->stop_time = time(NULL);
				instance->uptime = 0;
				instance->backoff = 0;
				instance->exitcode = 0;
				dprintf(STDERR_FILENO, "=> Instance %d of %s stopped\n",
					instance->id, prog->name);
			}
			else if (instance->state == E_RUNNING) // Exited self
			{
				instance->exitcode = WEXITSTATUS(status);
				instance->state = E_EXITED;
				instance->pid = 0;
				instance->start_time = 0;
				instance->stop_time = time(NULL);
				instance->uptime = 0;
				instance->backoff = 0;
				dprintf(STDERR_FILENO, "==> Instance %d of %s exited with code %d\n",
					instance->id, prog->name, instance->exitcode);
			}
			else
			{
				dprintf(STDERR_FILENO, "|%s|\n", get_instance_state(instance));
				assert(instance->state == E_STARTING); // Plus de protect
			}
			
		}
	}
}

int8_t      waiter(t_env *env)
{
    t_list      *list_ptr;
    t_program   *prog;
    t_instance  *instance;
    int         status;

    list_ptr = env->prgm_list;
    while (list_ptr != NULL) // Check all programs
    {
        prog = list_ptr->content;
        instance = prog->instance;
        while (instance != NULL)
        {
            status = 0;
			update_instance_uptime(instance);
			if (instance->state != E_STOPPED && instance->state != E_FATAL
				&& instance->state != E_EXITED && instance->state != E_BACKOFF)
           		if (waitpid(instance->pid, &status, WNOHANG | WUNTRACED | WCONTINUED))
                	terminate_instance(prog, instance, status);
			check_instance(prog, instance);
			instance = instance->next;
        }
        list_ptr = list_ptr->next;
    }
    return (0);
}
