/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_status.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:23:14 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 02:45:59 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_status(t_instance *instance, t_program *program)
{
	t_vector	*vct;
	char		*str;
	char		*state;

	vct = NULL;
	(void)program;
	if (instance != NULL && program != NULL)
	{
		state = get_instance_state(instance);
		str = ft_asprintf("%s\t\t%s\tpid %5d, uptime %d\n", instance->name,
					state, instance->pid, instance->uptime); // A FORMATER EM FONCTION DES STATUTS
		vct = vct_newstr(str);
		ft_strdel(&str);
	}
	return (vct);
}

t_vector			*cmd_status(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_status);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av, cmd->ac, action_status);
	return (vct);
}

