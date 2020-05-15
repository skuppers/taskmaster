/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_status.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:23:14 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 18:14:32 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static char			*status_running(t_instance *instance, char *state)
{
	struct tm	*timer;
	char		*str;

	str = NULL;
	timer = gmtime(&instance->uptime);
	if (timer != NULL)
	{
		if (instance->uptime < 3600)
			timer->tm_hour = 0;
		str = ft_asprintf("%-20s%-8spid: %5d | [uptime]\t% 9.2u:%.2u:%.2u\n",
				instance->name, state, instance->pid, timer->tm_hour,
				timer->tm_min, timer->tm_sec);
	}
	return (str);
}

static char			*status_stopped_exited(t_instance *instance, char *state)
{
	char		*tmp;
	char		*str;

	tmp = ft_asprintf("%-20s%-18s |", instance->name, state);
	if (instance->stop_time == 0)
		str = ft_asprintf("%s not started.\n", tmp,
					ctime(&instance->stop_time));
	else
		str = ft_asprintf("%s [stoptime] %s", tmp, ctime(&instance->stop_time));
	ft_strdel(&tmp);
	return (str);
}

t_vector			*action_status(t_instance *instance, t_program *program)
{
	t_vector	*vct;
	char		*str;
	char		*state;

	vct = NULL;
	str = NULL;
	(void)program;
	if (instance == NULL || program == NULL)
		return (NULL);
	state = get_instance_state(instance);
	if (instance->state == E_RUNNING)
		str = status_running(instance, state);
	else if (instance->state == E_STOPPED || instance->state == E_EXITED)
		str = status_stopped_exited(instance, state);
	else if (instance->state == E_FATAL)
		str = ft_asprintf("%-20s%s (logs may have details)\n", instance->name, state);
	else
		str = ft_asprintf("%-20s%s\n", instance->name, state);
	vct = vct_newstr(str);
	ft_strdel(&str);
	return (vct);
}

t_vector			*cmd_status(t_cmd *cmd)
{
	t_vector	*vct;
	t_vector	*tmp;

	vct = NULL;
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_status);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av, cmd->ac, action_status);
	else if (cmd->ocp == 0x03)
	{
		vct = progs_info(g_denv->prgm_list);
		if (g_tmpenv != NULL)
		{
			tmp = progs_info(g_tmpenv->prgm_list);
			vct_cat(vct, tmp);
			vct_del(&tmp);
		}
	}
	return (vct);
}
