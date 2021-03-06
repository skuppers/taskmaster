/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_remove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:32:31 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 15:50:56 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void	del_prog(void *d)
{
	t_program *program;

	program = d;
	ft_lstdel(&program->env, free_env);
	ft_strdel(&program->name);
	ft_free_tab_str(program->avs);
	ft_free_tab_str(program->exitcodes);
	free(program);
}

t_vector	*action_remove(t_instance *instance, t_program *program)
{
	char		*tmp;
	t_vector	*msg;

	(void)instance;
	tmp = ft_asprintf("%s", program->name);
	stop_prog(program);
	ft_lstdelnode(&g_denv->prgm_list, program, del_prog);
	msg = get_msg(tmp, "disappeared", INFO_MSG);
	ft_strdel(&tmp);
	return (msg);
}

t_vector	*cmd_remove(t_cmd *cmd)
{
	return (exec_action_args_group(cmd->av, cmd->ac, action_remove, g_denv));
}
