/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 15:16:49 by ffoissey         ###   ########.fr       */
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

FILE	*open_file(char *path, char *mode)
{
	FILE	*file;
	file = fopen(path, mode);
	if (file == NULL)
	{
		tlog(E_LOGLVL_ERRO, "taskmasterd: could not open %s: %s\n", path, strerror(errno));
		exit_routine();
	}
	return (file);
}

void	aggregate_stdout(t_program *pg, t_instance *in)
{
	FILE 		*stdoutfile;
	char		*asp;

	asp = NULL;
	if (ft_strequ(pg->stdout_logfile, "AUTO") == TRUE)
	{
		asp = ft_asprintf("%s/%s_%d_out.log", g_denv->opt.str[CHILDLOGDIR], pg->name, in->id);
		dprintf(STDERR_FILENO, "Creating %s\n", asp);
		stdoutfile = open_file(asp, "a+");
		ft_strdel(&asp);
		dup2(fileno(stdoutfile), STDOUT_FILENO);
		fclose(stdoutfile);
	}
	else if (ft_strequ(pg->stdout_logfile, "NONE") == TRUE)
		close(STDOUT_FILENO);
	else
	{
		asp = ft_asprintf("%s%d", pg->stdout_logfile, in->id);
		dprintf(STDERR_FILENO, "Creating %s\n", asp);
		stdoutfile = open_file(asp, "a+");
		ft_strdel(&asp);
		dup2(fileno(stdoutfile), STDOUT_FILENO);
		fclose(stdoutfile);
	}
}

void	aggregate_stderr(t_program *pg, t_instance *in)
{
	FILE 		*stderrfile;
	char		*asp;

	asp = NULL;

	if (ft_strequ(pg->stderr_logfile, "AUTO") == TRUE)
	{
		asp = ft_asprintf("%s/%s_%d_err.log", g_denv->opt.str[CHILDLOGDIR], pg->name, in->id);
		dprintf(STDERR_FILENO, "Creating %s\n", asp);
		stderrfile = open_file(asp, "a+");
		ft_strdel(&asp);
		dup2(fileno(stderrfile), STDERR_FILENO);
		fclose(stderrfile);
	}
	else if (ft_strequ(pg->stderr_logfile, "NONE") == TRUE)
		close(STDERR_FILENO);
	else
	{
		asp = ft_asprintf("%s%d", pg->stderr_logfile, in->id);
		dprintf(STDERR_FILENO, "Creating %s\n", asp);
		stderrfile = open_file(asp, "a+");
		ft_strdel(&asp);
		dup2(fileno(stderrfile), STDERR_FILENO);
		fclose(stderrfile);
	}
}

static void	change_uid(t_program *prog, t_instance *instance)
{
	if (prog->userid != -1 && setuid(prog->userid) != 0)
	{
		tlog(E_LOGLVL_ERRO, "instance %d of %s cant change uid: %s\n", instance->id, prog->name, strerror(errno));
		exit_routine();	
	}
}

static void	change_dir(t_program *prog, t_instance *instance)
{
	if (prog->directory != NULL && chdir(prog->directory) != 0)
	{
		tlog(E_LOGLVL_ERRO, "instance %d of %s cant change directory: %s\n", instance->id, prog->name, strerror(errno));
		exit_routine();
	}
}

int     child_process(t_program *prog, t_instance *instance, t_list *env)
{
	char	**environ;

	if (prog->pgid == 0)
		prog->pgid = getpid();
	setpgid(getpid(), prog->pgid);

	if (get_new_bin_path(prog, env) == FAILURE)
		tlog(E_LOGLVL_ERRO,
			"taskmasterd: Program %s instance %d pre-execution error: %s\n",
				prog->name, instance->id, "unvalid path or no right");

	concat_env_to_daemon_env(prog, env);
	environ = envtotab(prog->env);

	close(STDIN_FILENO);
	aggregate_stdout(prog, instance);
	aggregate_stderr(prog, instance);

	change_uid(prog, instance);
	change_dir(prog, instance);
	
    if (execve(prog->bin, prog->avs, environ) == FAILURE)
		tlog(E_LOGLVL_ERRO,
			"taskmasterd: Program %s instance %d execution error: %s\n",
				prog->name, instance->id, strerror(errno));
	ft_free_tab_str(environ);
	exit(1);
}

void	print_cmd_success(char *cmd, int ls, t_program *pg, uint8_t	nb)
{
	if (ls != 0)
		tlog(E_LOGLVL_ERRO, "Failed to %s instance %d of %s: ", cmd, nb, pg->name);
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
		tlog(E_LOGLVL_ERRO, "Instance %d of %s with pid %d entered %s state.\n",
							nb, pg->name, get_instance(pg, nb)->pid,
							get_instance_state(get_instance(pg, nb)));
	}
}

void    launch_jobs(t_denv *env)
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
		prog->pgid = 0;
		while (inst_nb < prog->numprocs)
		{
			inst = new_instance(inst_nb, prog->name);	// create instance meta
			if (inst == NULL)
			{
				tlog(E_LOGLVL_ERRO, "Failed to allocate instance\n");
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
