/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 15:29:42 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

t_env	*g_env;

void	print_dbg(t_env *env)
{
	t_list *ptr;
	
	for (ptr = env->prgm_list; ptr != NULL; ptr = ptr->next)
	{
		printf("Program: %s\n", ((t_program *)ptr->content)->name);
		printf(" - command:\t%s\n", ((t_program *)ptr->content)->command);
		printf(" - bin:\t%s\n", ((t_program *)ptr->content)->bin);
	//	printf(" - bin:\t%s\n", ((t_program *)ptr->content)->avs);
		printf(" - numprocs:\t%d\n", ((t_program *)ptr->content)->numprocs);
		printf(" - autostart:\t%d\n", ((t_program *)ptr->content)->autostart);
		printf(" - autorestart:\t%d\n", ((t_program *)ptr->content)->autorestart);
		printf(" - startsecs:\t%d\n", ((t_program *)ptr->content)->startsecs);
		printf(" - startretries:\t%d\n", ((t_program *)ptr->content)->startretries);
		printf(" - stopwaitsec:\t%d\n", ((t_program *)ptr->content)->stopwaitsecs);
		printf(" - exitcodes:\n");
		ft_print_strtab(((t_program *)ptr->content)->exitcodes);

		printf(" - directory:\t%s\n", ((t_program *)ptr->content)->directory);
		printf(" - umask:\t%u\n", ((t_program *)ptr->content)->umask);
		printf(" - priority:\t%d\n", ((t_program *)ptr->content)->priority);
		printf(" - stopsignals:\t%d\n", ((t_program *)ptr->content)->stopsignal);
		printf(" - user:\t\t%s\n", ((t_program *)ptr->content)->user);
		printf(" - redirect_stderr:\t%d\n", ((t_program *)ptr->content)->redirect_stderr);
		printf(" - stdout_logfile:\t%s\n", ((t_program *)ptr->content)->stdout_logfile);
		printf(" - stderr_logfile:\t%s\n", ((t_program *)ptr->content)->stderr_logfile);
		printf(" - environment:\t%s\n\n", ((t_program *)ptr->content)->environ);
	}
	for (ptr = env->group_list ; ptr != NULL; ptr = ptr->next)
	{
/*		printf("Group: %s\n", ((t_group *)ptr->content)->name);
		printf(" - programs: %s\n", ((t_group *)ptr->content)->programs);
		printf(" - priority: %d\n", ((t_group *)ptr->content)->priority);
*/
	}
}

void	set_daemon_environment(t_env *env, char **environ)
{
	env->environ = envtolst(environ);
	if (env->opt.environ == NULL)
		return ;
	strvalue_to_lst(&env->environ, env->opt.environ);
//	print_lst(env->environ); // DEBUG LIST ENV
}

int main(int ac, char **av, char **environ)
{
	t_env   env;

	ft_memset(&env, 0, sizeof(t_env));
	ft_memset(&env.opt, 0, sizeof(t_options));
	g_env = &env;
	set_taskmasterd_defautls(&env); // DOIT ABSOLUMENT ETRE FAIT EN PREMIER
	check_dflt_directory();
	get_opt(&env, ac - 1, av + 1);
	set_daemon_environment(&env, environ);
	if (init_log(&env) != 0)
		return (-1);

	parse_ini_file(&env, env.dict);
	
	if (ft_strequ(env.opt.str[LOGLEVEL], "debug") == 1)
		print_dbg(&env);

	init_signals();
	
	if (env.opt.optmask & OPT_NODAEMON)
	{
		if (make_socket(&env, DFL_SOCKET) != 0)
			return (-1);
		if (bind_socket(&env) != 0)
			return (-1);
		launch_jobs(&env);
		listen_for_data(&env);
	}
	else
		daemonize(&env);
	exit_routine();
}
