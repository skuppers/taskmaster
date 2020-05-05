/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/04 21:56:06 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void	concat_env_to_daemon_env(t_program *prog, t_list *env)
{
	t_list	*env_prog;

	if (prog->env == NULL)
	{
		prog->env = env;
		return ;
	}
	env_prog = prog->env;
	while (env_prog != NULL && env_prog->next != NULL)
		env_prog = env_prog->next;
	env_prog->next = env;
}

int     child_process(t_program *prog, t_instance *instance, t_list *env)
{
	char	**environ;
	
	concat_env_to_daemon_env(prog, env);	
	environ = envtotab(prog->env);
//	FILE *stderrfile;
//	FILE *stdoutfile;

//	dprintf(STDERR_FILENO, "Prog %s pid %d pgid %d\n", prog->process_name, prog->pid, prog->pgid);

/*	stdoutfile = fopen( prog->stdout_logfile, "a+" );
	stderrfile = fopen( prog->stderr_logfile, "a+" );
    if (stdoutfile == NULL || stderrfile == NULL)
	{
        fputs( "Could not open file", stderr );
        exit (1);
    }
    dup2(fileno(stdoutfile), STDOUT_FILENO);
	dup2(fileno(stderrfile), STDERR_FILENO);
	fclose(stdoutfile);
	fclose(stderrfile);*/

    if (execve(prog->bin, prog->avs, environ) == FAILURE)
		ft_dprintf(2, "taskmasterd: Program %s instance %d execution error: %s\n",
				prog->name, instance->id, strerror(errno));
	ft_free_tab_str(environ);
	exit(1);
}

void	print_cmd_success(char *cmd, int ls, t_program *pg, uint8_t	nb)
{
	if (ls != 0)
		tlog(g_env, E_LOGLVL_ERRO, "Failed to %s instance %d of %s: ", cmd, nb, pg->name);
	if (ls == ERR_STARTING)
		tlog(g_env, E_LOGLVL_ERRO, "instance is already starting.\n");
	else if (ls == ERR_RUNNING)
		tlog(g_env, E_LOGLVL_ERRO, "instance is already running.\n");
	else if (ls == ERR_STOPPING)
		tlog(g_env, E_LOGLVL_ERRO, "instance is already stopping.\n");
	else if (ls == ERR_STOPPED)
		tlog(g_env, E_LOGLVL_ERRO, "instance is already stopped.\n");
	else if (ls == ERR_FORK)
		tlog(g_env, E_LOGLVL_ERRO, "fork() failed.\n");
	else if (ls == ERR_EXEC)
		tlog(g_env, E_LOGLVL_ERRO, "execve() failed.\n");
	else if (ls == ERR_UNDEF_INST)
		tlog(g_env, E_LOGLVL_ERRO, "undefined instance\n");
	else if (ls == 0)
	{
		tlog(g_env, E_LOGLVL_ERRO, "Instance %d of %s with pid %d entered %s state.\n",
							nb, pg->name, get_instance(pg, nb)->pid,
							get_instance_state(get_instance(pg, nb)));
	}
}

void    launch_jobs(t_env *env)
{
    t_list      *ptr;
    t_program   *prog;
	uint8_t		inst_nb;
	t_instance	*inst;
	int			launch_success;

    ptr = env->prgm_list;
    while (ptr != NULL)
    {
		inst_nb = 0;
        prog = ptr->content;
		while (inst_nb < prog->numprocs)
		{
			inst = new_instance(inst_nb, prog->name);	// create instance meta
			if (inst == NULL)
			{
				tlog(g_env, E_LOGLVL_ERRO, "Failed to allocate instance\n");
				break ;
			}
			add_instance(prog, inst);		//add instance to program_list
			if (prog->autostart == TRUE)
			{
				launch_success = start_instance(prog, inst_nb, env->environ);
				print_cmd_success("start", launch_success, prog, inst_nb);
			}
			++inst_nb;
		}
		ptr = ptr->next;
	}
}
