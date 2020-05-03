/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 18:46:18 by ffoissey         ###   ########.fr       */
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
	if (instance->state == E_STARTING && instance->uptime >= prog->startsec)
	{
		instance->state = E_RUNNING;
   		dprintf(STDERR_FILENO, "Instance %d of %s entered %s state after %d secs\n",
                instance->id, prog->name, STATE_RUNNING, instance->uptime);
	}
	else if (instance->state == E_STOPPING
		&& instance->uptime >= (instance->stop_time + prog->stopwaitsec))
	{
		kill(instance->pid, SIGKILL);
		instance->state = E_STOPPED;
		instance->pid = 0;
		instance->start_time = 0;
		instance->stop_time = time(NULL);
		dprintf(STDERR_FILENO, "Instance %d of %s %s by forced exit\n",
                instance->id, prog->name, get_instance_state(instance));
	}
	else if (instance->state == E_BACKOFF)
	{
		int ret = start_instance(prog, instance->id, g_env->environ);
		print_cmd_success("(re)start", ret, prog, instance->id);
	}
}

void        terminate_instance(t_program *prog, t_instance *instance, int status)
{
    int exit_code;
	
	if (WIFSTOPPED(status))
		instance_stopped(prog, instance);
	else if (WIFCONTINUED(status))
		instance_continued(prog, instance);
	else
	{
		//exit_code = WEXITSTATUS(status);
		//dprintf(STDERR_FILENO, "=> Instance %d from %s exited with code %d\n",
		//	instance->id, prog->name, exit_code);
		if (instance->uptime < prog->startsec)
		{
			if (instance->backoff >= prog->startretries)
			{
				dprintf(STDERR_FILENO, "=====> Instance %d from %s entered FATAL state.\n",
					instance->id, prog->name);
				instance->state = E_FATAL;
				instance->backoff = 0;
			}
			else
			{
				dprintf(STDERR_FILENO, "===> Instance %d from %s backoff.\n",
				instance->id, prog->name);
				instance->state = E_BACKOFF;
				instance->backoff++;
			}
			instance->pid = 0;
			instance->start_time = 0;
			instance->stop_time = time(NULL);
			instance->uptime = 0;
		}
		else //expected ?  { restart x times }
		{
			instance->state = E_STOPPED;
			instance->pid = 0;
			instance->start_time = 0;
		}
	}
}

int8_t      waiter(t_env *env)
{
    t_list      *list_ptr;
    t_program   *prog;
    t_instance  *instance;
    uint8_t     all_progs_checked;
    int         status;

    list_ptr = env->prgm_list;
    all_progs_checked = 0;
    while (list_ptr != NULL) // Check all programs
    {
        prog = list_ptr->content;
        instance = prog->instance;
        while (instance != NULL)
        {
			// TESTS
	/*		if (instance->uptime >= 3 && instance->state != E_STOPPED)
			{
				int sc = stop_instance(prog, instance);
				print_cmd_success("stop", sc, prog, instance->id);
			}
*/
            status = 0;
			update_instance_uptime(instance);
			if (instance->state != E_STOPPED)
           		if (waitpid(instance->pid, &status, WNOHANG | WUNTRACED | WCONTINUED))
                	terminate_instance(prog, instance, status);
			check_instance(prog, instance);
            
			instance = instance->next;
        }
        list_ptr = list_ptr->next;
    }
    return (0);
}
