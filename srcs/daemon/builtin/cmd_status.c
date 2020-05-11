/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_status.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:23:14 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/11 18:18:18 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_status(t_instance *instance, t_program *program)
{
	t_vector	*vct;
	char		*str;
	char		*state;
	struct tm	*timer;
	char 		*tmp;

	vct = NULL;
	str = NULL;
	(void)program;
	if (instance != NULL && program != NULL)
	{
		state = get_instance_state(instance);
		if (instance->state == E_RUNNING)
		{
			timer = gmtime(&instance->uptime);
			if (timer != NULL)
			{
				if (instance->uptime < 3600)
					timer->tm_hour = 0;
				str = ft_asprintf("%-20s%-8spid: %5d | [uptime]\t% 9.2u:%.2u:%.2u\n",
						instance->name, state, instance->pid, timer->tm_hour,
						timer->tm_min, timer->tm_sec);
			}
		}
		else if (instance->state == E_STOPPED || instance->state == E_EXITED)
		{
			tmp = ft_asprintf("%-20s%-18s |", instance->name, state);
			if (instance->stop_time == 0)
				str = ft_asprintf("%s not started.\n", tmp, ctime(&instance->stop_time));
			else
				str = ft_asprintf("%s [stoptime] %s", tmp, ctime(&instance->stop_time));
		}
		else
			str = ft_asprintf("%-20s%s\n", instance->name, state);
			
		vct = vct_newstr(str);
		ft_strdel(&str);
	}
	return (vct);
}

static t_vector			*progs_info(void)
{
	t_list		*prog_list;
	t_program	*program;
	t_instance	*instance;
	t_vector	*output;

	prog_list = g_denv->prgm_list;
	output = vct_new(0);
	while (prog_list != NULL)
	{
		program = (t_program *)prog_list->content;
		if (program != NULL)
		{
			if (program->numprocs > 0)
			{
				if (prog_list != g_denv->prgm_list)
					vct_add(output, '/');
				vct_addstr(output, program->name);
				vct_add(output, '/');
				vct_addstr(output, program->name);
				vct_addstr(output, ":*;");
			}
			instance = program->instance;
			while (instance != NULL)
			{
				vct_addstr(output, instance->name);
				instance = instance->next;
				if (instance != NULL)
					vct_add(output, ';');
			}
		}
		prog_list = prog_list->next;
	}
	return (output);
}

t_vector			*cmd_status(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_status);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av, cmd->ac, action_status);
	else if (cmd->ocp == 0x03)
		vct = progs_info();
	return (vct);
}

