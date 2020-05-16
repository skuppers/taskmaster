/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 14:37:33 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/16 15:23:28 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void	aggregate_std(t_program *pg, t_instance *in, const int fd)
{
	FILE		*stdfile;
	char		*asp;
	char		*logfile;

	asp = NULL;
	logfile = (fd == STDOUT_FILENO) ? pg->stdout_logfile : pg->stderr_logfile;
	if (ft_strequ(logfile, "AUTO") == true)
		asp = ft_asprintf("%s/%s_%d_%s.log", g_denv->opt.str[CHILDLOGDIR],
							pg->name, in->id,
							(fd == STDOUT_FILENO) ? "out" : "err");
	else if (ft_strequ(logfile, "NONE") == true)
		close(fd);
	else
		asp = ft_asprintf("%s%d", logfile, in->id);
	if (asp != NULL)
	{
		stdfile = fopen(asp, "a+");
		ft_strdel(&asp);
		if (stdfile == NULL)
			exit(EXIT_FAILURE);
		dup2(fileno(stdfile), fd);
		fclose(stdfile);
		ft_putendl_fd("- - - - - - - - - - - - - - - - - - - - - - - - -", fd);
	}
}

static void	change_uid_dir(t_program *prog, t_instance *instance)
{
	if (prog->userid != -1 && setuid(prog->userid) != 0)
	{
		tlog(E_LOGLVL_ERRO, "taskmasterd: instance %d of %s cant change uid\n",
				instance->id, prog->name);
		exit(FAILURE);
	}
	if (prog->directory != NULL && chdir(prog->directory) != 0)
	{
		tlog(E_LOGLVL_ERRO,
				"taskmasterd: instance %d of %s cant change directory\n",
				instance->id, prog->name);
		exit(FAILURE);
	}
}

static void	reset_signal(void)
{
	signal(SIGTERM, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGUSR1, SIG_DFL);
	signal(SIGUSR2, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGHUP, SIG_DFL);
}

static void	redirect_process(t_program *prog, t_instance *instance)
{
	dup2(instance->fd[CHILD_STDIN], STDIN_FILENO);
	dup2(instance->fd[CHILD_STDOUT], STDOUT_FILENO);
	dup2(instance->fd[CHILD_STDERR], STDERR_FILENO);
	close_child_fd(instance);
	aggregate_std(prog, instance, STDOUT_FILENO);
	aggregate_std(prog, instance, STDERR_FILENO);
}

void		child_process(t_program *prog, t_instance *instance, t_list *env)
{
	char	**environ;

	if (prog->pgid == 0)
		prog->pgid = getpid();
	tcsetpgrp(STDIN_FILENO, 0);
	redirect_process(prog, instance);
	reset_signal();
	setpgid(getpid(), prog->pgid);
	if (get_new_bin_path(&prog->bin,
				vct_newstr(get_var(env, "PATH"))) == FAILURE)
		tlog(E_LOGLVL_ERRO,
			"taskmasterd: Program %s instance %d pre-execution error: %s\n",
				prog->name, instance->id, "unvalid path or no right");
	prog->avs[0] = prog->bin;
	concat_env_to_daemon_env(prog, env);
	environ = envtotab(prog->env);
	change_uid_dir(prog, instance);
	if (execve(prog->bin, prog->avs, environ) == FAILURE)
		tlog(E_LOGLVL_ERRO,
			"taskmasterd: Program %s instance %d execution error: %s\n",
				prog->name, instance->id, strerror(errno));
	ft_free_tab_str(environ);
	close_parrent_fd(instance);
	close_child_fd(instance);
	exit(EXIT_FAILURE);
}
