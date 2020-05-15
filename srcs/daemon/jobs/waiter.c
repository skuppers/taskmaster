/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 13:17:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/15 17:00:35 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void		reinit(t_instance *instance, enum e_prg_state new_state,
					uint8_t flag, int exit_code)
{
	instance->state = new_state;
	if ((flag & NO_RESET) == FALSE)
	{
		close_parrent_fd(instance);
		close_child_fd(instance);
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
		if (kill(instance->pid, SIGKILL) == FAILURE)
			tlog(E_LOGLVL_ERRO, "failed to kill instance %s: %s\n",
				instance->name, strerror(errno));
		else
			reinit(instance, E_STOPPED, KEEP_BACKOFF, 0);
	}
	else if (instance->state == E_BACKOFF)
	{
		start_instance(prog, instance->id, g_denv->environ);
		log_state_information(instance);
	}
	else if (instance->state == E_EXITED && prog->autorestart != FALSE)
	{
		if (prog->autorestart == TRUE || (prog->autorestart == UNEXPECTED
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
		reinit(instance, E_STOPPED, NO_RESET, 0);
	else if (WIFCONTINUED(status))
		reinit(instance, E_RUNNING, NO_RESET, 0);
	else
	{
		if (instance->uptime < prog->startsecs)
		{
			if (instance->backoff >= prog->startretries)
				reinit(instance, E_FATAL, FULL_RESET, 0);
			else
				reinit(instance, E_BACKOFF, INC_BACKOFF, 0);
		}
		else
		{
			if (instance->state == E_STOPPING)
				reinit(instance, E_STOPPED, FULL_RESET, 0);
			else if (instance->state == E_RUNNING)
				reinit(instance, E_EXITED, FULL_RESET, WEXITSTATUS(status));
			else if (instance->state == E_STARTING)
				tlog(E_LOGLVL_INFO, "instance %s starting...\n",
					instance->name, get_instance_state(instance));
		}
	}
}

void 			instance_waiter(t_program *prog, t_instance *instance)
{
	int 		status;

	status = 0;
	update_instance_uptime(instance);
	check_instance(prog, instance);
	if (instance->state == E_STARTING || instance->state == E_RUNNING
			|| instance->state == E_STOPPING)
		if (waitpid(instance->pid, &status,
				WNOHANG | WUNTRACED | WCONTINUED))
			terminate_instance(prog, instance, status);
	
}

void			waiter(void)
{
	t_list		*list_ptr;
	t_program	*prog;
	t_instance	*instance;

	list_ptr = g_denv->prgm_list;
	while (list_ptr != NULL)
	{
		prog = list_ptr->content;
		instance = prog->instance;
		while (instance != NULL)
		{
			instance_waiter(prog, instance);
			instance = instance->next;
		}
		list_ptr = list_ptr->next;
	}
}
