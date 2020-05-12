/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_tail.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:26:58 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 23:28:26 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_tail_err(t_instance *instance, t_program *program)
{
	char		*asp;	
	t_vector	*vct;
	size_t		len;

	if (g_denv->opt.str[CHILDLOGDIR] == NULL)
		return (get_msg(instance->name, "no log file", ERR_MSG));
	len = ft_strlen(g_denv->opt.str[CHILDLOGDIR]);
	if (g_denv->opt.str[CHILDLOGDIR][len - 1] == '/')
		asp = ft_asprintf("%s%s_%d_err.log", g_denv->opt.str[CHILDLOGDIR],
						program->name, instance->id);
	else
		asp = ft_asprintf("%s/%s_%d_err.log", g_denv->opt.str[CHILDLOGDIR],
						program->name, instance->id);
	vct = vct_newstr(asp);
	ft_strdel(&asp);
	vct_add(vct, ETX);
	return (vct);
}

t_vector	*action_tail_out(t_instance *instance, t_program *program)
{
	char		*asp;	
	t_vector	*vct;
	size_t		len;

	if (g_denv->opt.str[CHILDLOGDIR] == NULL)
		return (get_msg(instance->name, "no log file", ERR_MSG));
	len = ft_strlen(g_denv->opt.str[CHILDLOGDIR]);
	if (g_denv->opt.str[CHILDLOGDIR][len - 1] == '/')
		asp = ft_asprintf("%s%s_%d_out.log", g_denv->opt.str[CHILDLOGDIR],
						program->name, instance->id);
	else
		asp = ft_asprintf("%s/%s_%d_out.log", g_denv->opt.str[CHILDLOGDIR],
						program->name, instance->id);
	vct = vct_newstr(asp);
	ft_strdel(&asp);
	vct_add(vct, ETX);
	return (vct);
}

t_vector			*cmd_tail(t_cmd *cmd)
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
