/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_clear.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:29:08 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 17:39:00 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*process_clear(t_instance *instance, t_program *program, int fd)
{
	t_vector	*vct;
	FILE		*file;

	vct = get_logfile_name(program, instance, fd, ETX);
	if (vct_getlastchar(vct) == ETX)
	{
		vct_cut(vct);
		if ((file = fopen(vct_getstr(vct), "w")) == NULL)
		{
			vct_del(&vct);
			vct = get_msg(instance->name, (fd == STDERR_FILENO) ?
					"stderr: no log file" : "stdout: no log file", ERR_MSG);
		}
		else
		{
			fclose(file);
			vct_del(&vct);
			vct = get_msg(instance->name, (fd == STDERR_FILENO) ?
					"stderr: log file cleared" : "stdout: log file cleared",
					INFO_MSG);
		}
	}
	return (vct);
}

t_vector	*action_clear(t_instance *instance, t_program *program)
{
	t_vector	*vct;
	t_vector	*output;

	output = process_clear(instance, program, STDOUT_FILENO);
	vct = process_clear(instance, program, STDERR_FILENO);
	vct_cat(output, vct);
	vct_del(&vct);
	return (output);
}

t_vector	*cmd_clear(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_clear);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av, cmd->ac, action_clear);
	return (vct);
}
