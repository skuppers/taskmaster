/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 21:21:39 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

t_denv	*g_denv;

void	print_dbg(t_denv *env)
{
	t_list *ptr;
	
	for (ptr = env->prgm_list; ptr != NULL; ptr = ptr->next)
	{
		dprintf(STDERR_FILENO, "Program: %s\n", ((t_program *)ptr->content)->name);
		dprintf(STDERR_FILENO, " - command:\t%s\n", ((t_program *)ptr->content)->command);
		dprintf(STDERR_FILENO, " - bin:\t%s\n", ((t_program *)ptr->content)->bin);
	//	dprintf(STDERR_FILENO, " - bin:\t%s\n", ((t_program *)ptr->content)->avs);
		dprintf(STDERR_FILENO, " - numprocs:\t%d\n", ((t_program *)ptr->content)->numprocs);
		dprintf(STDERR_FILENO, " - autostart:\t%d\n", ((t_program *)ptr->content)->autostart);
		dprintf(STDERR_FILENO, " - autorestart:\t%d\n", ((t_program *)ptr->content)->autorestart);
		dprintf(STDERR_FILENO, " - startsecs:\t%d\n", ((t_program *)ptr->content)->startsecs);
		dprintf(STDERR_FILENO, " - startretries:\t%d\n", ((t_program *)ptr->content)->startretries);
		dprintf(STDERR_FILENO, " - stopwaitsec:\t%d\n", ((t_program *)ptr->content)->stopwaitsecs);
		dprintf(STDERR_FILENO, " - exitcodes:\n");
		ft_print_strtab(((t_program *)ptr->content)->exitcodes);

		dprintf(STDERR_FILENO, " - directory:\t%s\n", ((t_program *)ptr->content)->directory);
		dprintf(STDERR_FILENO, " - umask:\t%u\n", ((t_program *)ptr->content)->umask);
		dprintf(STDERR_FILENO, " - priority:\t%d\n", ((t_program *)ptr->content)->priority);
		dprintf(STDERR_FILENO, " - stopsignals:\t%d\n", ((t_program *)ptr->content)->stopsignal);
		dprintf(STDERR_FILENO, " - user:\t\t%d\n", ((t_program *)ptr->content)->userid);
		dprintf(STDERR_FILENO, " - redirect_stderr:\t%d\n", ((t_program *)ptr->content)->redirect_stderr);
		dprintf(STDERR_FILENO, " - stdout_logfile:\t%s\n", ((t_program *)ptr->content)->stdout_logfile);
		dprintf(STDERR_FILENO, " - stderr_logfile:\t%s\n", ((t_program *)ptr->content)->stderr_logfile);
		dprintf(STDERR_FILENO, " - environment:\t%s\n\n", ((t_program *)ptr->content)->environ);
	}
}

void	set_daemon_environment(t_denv *env, char **environ)
{
	env->environ = envtolst(environ);
	if (env->opt.environ == NULL)
		return ;
	strvalue_to_lst(&env->environ, env->opt.environ);
//	print_lst(env->environ); // DEBUG LIST ENV
}

int main(int ac, char **av, char **environ)
{
	t_denv   env;

	ft_memset(&env, 0, sizeof(t_denv));
	ft_memset(&env.opt, 0, sizeof(t_options));
	g_denv = &env;
	set_taskmasterd_defautls(&env); // DOIT ABSOLUMENT ETRE FAIT EN PREMIER
	check_dflt_directory();
	get_opt(&env, ac - 1, av + 1);
	set_daemon_environment(&env, environ);
	if (init_log() != SUCCESS)
		return (EXIT_FAILURE);

	parse_ini_file(&env, env.dict);
	
	if (ft_strequ(env.opt.str[LOGLEVEL], "debug") == 1)
		print_dbg(&env);

	init_signals();

	if (env.opt.optmask & OPT_NODAEMON)
	{
		if (make_socket(&env, DFL_SOCKET) != 0)
			return (EXIT_FAILURE);
		if (bind_socket(&env) != 0)
			return (EXIT_FAILURE);
		launch_jobs(&env);
		listen_for_data(&env);
	}
	else
		daemonize(&env);
	exit_routine();
	return (EXIT_SUCCESS);
}
