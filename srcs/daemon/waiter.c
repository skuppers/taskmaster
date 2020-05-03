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
    instance->start_time = 0; //put stop time in here
    instance->state = STATE_STOPPED;
    dprint(STDERR_FILENO, "Instance %d of %s entered %s state.\n",
                instance->id, prog->name, STATE_STOPPED);
}

void    instance_continued(t_program *prog, t_instance *instance)
{
    instance->start_time = time(NULL);
    instance->state = STATE_RUNNING;
    dprint(STDERR_FILENO, "Instance %d of %s entered %s state.\n",
                instance->id, prog->name, STATE_RUNNING);
}

void        update_instance(t_program *prog, t_instance *instance, int status)
{
    int exit_code;
    
    exit_code = WEXITSTATUS(status);
    if (exit_code != 0)
        ; //expected ?  { restart x times }

    if (WIFSTOPPED(status))
		instance_stopped(prog, instance);
	if (WIFCONTINUED(status))
		instance_continued(prog, instance);
}

int8_t      waiter(t_env *env)
{
    t_list      *list_ptr;
    t_program   *prog;
    t_instance  *instance;
    uint8_t     all_progs_checked;
    pid_t       pid;
    int status;

    list_ptr = env->prgm_list;
    all_progs_checked = 0;
    while (list_ptr != NULL) // Check all programs
    {
        prog = list_ptr->content;
        instance = prog->instance;
        while (instance != NULL)
        {
            if (waitpid(instance->pid, &status, WNOHANG | WUNTRACED | WCONTINUED))
                update_instance(prog, instance, status);
            instance = instance->next;
        }
        list_ptr = list_ptr->next;
    }
}