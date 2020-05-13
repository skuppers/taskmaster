/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_add.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:28:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 15:03:19 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_add(t_instance *instance, t_program *program)
{
	(void)instance;
	
	dprintf(STDERR_FILENO, "adding %s\n", program->name);
	append_to_pgrmlist(g_denv, program);
	program->availmode = E_LOCKED;

	uint8_t		inst_nb;
	t_instance	*inst;

	inst_nb = 0;
	while (inst_nb < program->numprocs)
	{
		inst = new_instance(inst_nb, program->name);	// create instance meta
		if (inst == NULL)
		{
			tlog(E_LOGLVL_ERRO, "Failed to allocate instance\n");
			break ;
		}
		add_instance(program, inst);		//add instance to program_list
		if (program->autostart == TRUE)
		{
			int launch_success = start_instance(program, inst_nb, g_denv->environ);
			print_cmd_success("start", launch_success, program, inst_nb);
		}
		++inst_nb;
	}
	return (get_msg(program->name, "added", INFO_MSG));
}

t_vector	*cmd_add(t_cmd *cmd)
{
	return (exec_action_args_group(cmd->av, cmd->ac, action_add));
}
