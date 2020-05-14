/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tail.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:26:58 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 16:13:09 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_tail_err(t_instance *instance, t_program *program)
{
	return (get_logfile_name(program, instance, STDERR_FILENO, ETX));
}

t_vector	*action_tail_out(t_instance *instance, t_program *program)
{
	return (get_logfile_name(program, instance, STDOUT_FILENO, ETX));
}

t_vector	*cmd_tail(t_cmd *cmd)
{
	t_vector	*vct;
	char		*str;
	t_cmd		cmd_cpy;

	(void)cmd;
	cmd_cpy.ac = cmd->ac - 1;
	cmd_cpy.av = cmd->av;
	str = ft_strdup(cmd->av[cmd->ac - 1]);
	ft_strdel(&cmd->av[cmd->ac - 1]);
	cmd->ac--;
	if (cmd->ocp == 0x02)
	{
		cmd_cpy.av += 1;
		cmd_cpy.ac -= 1;
	}
	if (ft_strequ(cmd_cpy.av[0] + ft_strlen(cmd_cpy.av[0]) - 2, ":*") == true)
		vct = get_msg(cmd_cpy.av[0], "no such process", ERR_MSG);
	else if (ft_strequ(str, "stderr") == true)
		vct = exec_action_args(cmd_cpy.av, cmd_cpy.ac, action_tail_err);
	else
		vct = exec_action_args(cmd_cpy.av, cmd_cpy.ac, action_tail_out);
	ft_strdel(&str);
	return (vct);
}
