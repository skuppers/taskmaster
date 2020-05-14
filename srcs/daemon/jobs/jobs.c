/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 15:35:37 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void		print_cmd_success(char *cmd, int ls, t_program *pg, uint8_t nb)
{
	if (ls != 0)
		tlog(E_LOGLVL_ERRO, "Failed to %s instance %d of %s: ", cmd, nb,
				pg->name);
	if (ls == ERR_STARTING)
		tlog(E_LOGLVL_ERRO, "instance is already starting.\n");
	else if (ls == ERR_RUNNING)
		tlog(E_LOGLVL_ERRO, "instance is already running.\n");
	else if (ls == ERR_STOPPING)
		tlog(E_LOGLVL_ERRO, "instance is already stopping.\n");
	else if (ls == ERR_STOPPED)
		tlog(E_LOGLVL_ERRO, "instance is already stopped.\n");
	else if (ls == ERR_FORK)
		tlog(E_LOGLVL_ERRO, "fork() failed.\n");
	else if (ls == ERR_EXEC)
		tlog(E_LOGLVL_ERRO, "execve() failed.\n");
	else if (ls == ERR_UNDEF_INST)
		tlog(E_LOGLVL_ERRO, "undefined instance\n");
	else if (ls == 0)
	{
		tlog(E_LOGLVL_INFO, "Instance %d of %s with pid %d entered %s state.\n",
							nb, pg->name, get_instance(pg, nb)->pid,
							get_instance_state(get_instance(pg, nb)));
	}
}

static void	launch_instance(t_program *prog, uint16_t inst_nb)
{
	t_instance	*inst;
	int			launch_success;

	inst = new_instance(inst_nb, prog->name); // create instance meta
	if (inst == NULL) // create instance meta
		tlog(E_LOGLVL_ERRO, "Failed to allocate instance\n");
	else
	{
		add_instance(prog, inst);		//add instance to program_list
		if (prog->autostart == TRUE)
		{
			launch_success = start_instance(prog, inst_nb, g_denv->environ);
			print_cmd_success("start", launch_success, prog, inst_nb);
		}
	}
}

void		launch_jobs(void)
{
    t_list      *ptr;
    t_program   *prog;
	uint16_t	inst_nb;

    ptr = g_denv->prgm_list;
    while (ptr != NULL)
    {
		inst_nb = 0;
        prog = ptr->content;
		prog->pgid = 0;
		while (inst_nb < prog->numprocs)
		{
			launch_instance(prog, inst_nb);
			++inst_nb;
		}
		ptr = ptr->next;
	}
}
