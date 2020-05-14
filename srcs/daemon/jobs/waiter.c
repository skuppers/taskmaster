/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 13:17:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 15:32:20 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void		log_state_information(t_instance *instance)
{
	if (instance->state == E_EXITED)
		tlog(E_LOGLVL_INFO, "==> Instance %s exited with code %d\n",
			instance->name, instance->exitcode);
	else if (instance->state == E_BACKOFF || instance->state == E_STARTING)
		tlog(E_LOGLVL_INFO, "Instance %s with pid %d entered %s state.\n",
				instance->name, instance->pid, get_instance_state(instance));
	else if (instance->state == E_STOPPED)
		tlog(E_LOGLVL_INFO, "Instance %s %s by forced exit\n",
				instance->name, get_instance_state(instance));
	else if (instance->state == E_RUNNING)
		tlog(E_LOGLVL_INFO,
				"Instance %s entered %s state after %d secs\n",
				instance->name, STATE_RUNNING, instance->uptime);
	else if (instance->state == E_FATAL)
		tlog(E_LOGLVL_INFO,
				"=====> Instance %s entered FATAL state.\n", instance->name);
}

static void		reinit(t_instance *instance, enum e_prg_state new_state,
					uint8_t flag, int exit_code)
{
	instance->state = new_state;
	if ((flag & NO_RESET) == FALSE)
	{
		instance->pid = 0;
		instance->start_time = 0;
		instance->stop_time = time(NULL);
		if (flag & FULL_RESET)
		{
			instance->uptime = 0;
			instance->exitcode = exit_code;
		}
		if (flag & INC_BACKOFF)
			instance->backoff++;
		else if ((flag & KEEP_BACKOFF) == FALSE)
			instance->backoff = 0;
	}
	log_state_information(instance);
}

void			check_instance(t_program *prog, t_instance *instance)
{
	if (instance->state == E_STARTING && instance->uptime >= prog->startsecs)
		reinit(instance, E_RUNNING, NO_RESET, 0);
	else if (instance->state == E_STOPPING
		&& instance->uptime >= (instance->stop_time + prog->stopwaitsecs))
	{
		kill(instance->pid, SIGKILL); //TODO protect
		reinit(instance, E_STOPPED, KEEP_BACKOFF, 0);
	}
	else if (instance->state == E_BACKOFF)
	{
		start_instance(prog, instance->id, g_denv->environ);
		log_state_information(instance);
	}
	else if (instance->state == E_EXITED && prog->autorestart != FALSE)
	{
		if (prog->autorestart == TRUE
				|| (prog->autorestart == UNEXPECTED
					&& is_expected_exitcode(prog, instance) == 0))
		{
			start_instance(prog, instance->id, g_denv->environ);
			log_state_information(instance);
		}
	}
}

static void		terminate_instance(t_program *prog, t_instance *instance,
					int status)
{
	if (WIFSTOPPED(status))
		reinit(instance, E_STOPPED, NO_RESET, 0); // Ecriture sur terminal ? tcsetpgrp()
	else if (WIFCONTINUED(status))
		reinit(instance, E_RUNNING, NO_RESET, 0);
	else
	{
		if (instance->uptime < prog->startsecs) // Crash avant running state
		{
			if (instance->backoff >= prog->startretries)
				reinit(instance, E_FATAL, FULL_RESET, 0);
			else
				reinit(instance, E_BACKOFF, INC_BACKOFF, 0);
		}
		else // was in RUNNING state
		{
			if (instance->state == E_STOPPING) // stopped by user dont restart
				reinit(instance, E_STOPPED, FULL_RESET, 0); // SIGCHLD proper way
			else if (instance->state == E_RUNNING) // Exited self
				reinit(instance, E_EXITED, FULL_RESET, WEXITSTATUS(status));
			else if (instance->state == E_STARTING)
				tlog(E_LOGLVL_INFO, "instance %s starting...\n",
					instance->name, get_instance_state(instance));
			//// else 	ERROR ? 
		}
	}
}

void			waiter(void)
{
	t_list		*list_ptr;
	t_program	*prog;
	t_instance	*instance;
	int			status;

	list_ptr = g_denv->prgm_list;
	while (list_ptr != NULL) // Check all programs
	{
		prog = list_ptr->content;
		instance = prog->instance;
		while (instance != NULL)
		{
			status = 0;
			update_instance_uptime(instance);
			if (instance->state != E_STOPPED && instance->state != E_FATAL
					&& instance->state != E_EXITED
					&& instance->state != E_BACKOFF)
				if (waitpid(instance->pid, &status,    // if imbriqué ???
						WNOHANG | WUNTRACED | WCONTINUED))
					terminate_instance(prog, instance, status);
			check_instance(prog, instance);
			instance = instance->next;
		}
		list_ptr = list_ptr->next;
	}
}
