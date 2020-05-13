/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:33:41 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 15:14:29 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_denv				*g_newenv;

t_vector		*update_file(t_instance *in, t_program *prg)
{
	(void)in;(void)prg;
	t_list	*ptr;


	ptr = g_newenv->prgm_list;
	if (ptr == NULL)
		return (vct_newstr("Nothing to update.\n"));

	// program count
	// 
//	g_newenv = NULL;
	return (vct_newstr("Updated config file.\n"));
}

t_vector	*action_update(t_instance *instance, t_program *program)
{
	(void)instance;	
	return (get_msg(program->name, "unknown error", ERR_MSG));
}

t_vector			*cmd_update(t_cmd *cmd)
{
	//if (cmd->ocp == 0x01 || cmd->ocp == 0x02)
	//	return (exec_action_all_groups(action_update));
	return (exec_action_args_group(cmd->av, cmd->ac, action_update, g_denv));
}
