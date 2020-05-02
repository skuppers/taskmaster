/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 19:05:55 by ffoissey         ###   ########.fr       */
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
		printf(" - numprocs: %d\n", ((t_program *)ptr->content)->numprocs);
		printf(" - directory: %s\n", ((t_program *)ptr->content)->directory);
		printf(" - umask: %u\n", ((t_program *)ptr->content)->umask);
		printf(" - priority: %d\n", ((t_program *)ptr->content)->priority);
		printf(" - autostart: %d\n\n", ((t_program *)ptr->content)->autostart);
	}
	for (ptr = env->goup_list ; ptr != NULL; ptr = ptr->next)
	{
		printf("Group: %s\n", ((t_group *)ptr->content)->name);
		printf(" - programs: %s\n", ((t_group *)ptr->content)->programs);
		printf(" - priority: %d\n", ((t_group *)ptr->content)->priority);
	}
}

int main(int ac, char **av)
{
	t_env   env;

	ft_memset(&env, 0, sizeof(t_env));
	ft_memset(&env.opt, 0, sizeof(t_options));
	g_env = &env;

	set_taskmasterd_defautls(&env); // DOIT ABSOLUMENT ETRE FAIT EN PREMIER
	check_dflt_directory();

	get_opt(&env, ac - 1, av + 1);

	if (init_log(&env) != 0)
		return (-1);

	parse_ini_file(&env, env.dict);

	print_dbg(&env);

	init_signals();

	// make network things
	if (make_socket(&env, DFL_SOCKET) != 0)
		return (-1);
	if (bind_socket(&env) != 0)
		return (-1);

	//once all is ready, daemonize
	// And listen for incoming connections

	listen_for_data(&env);
	
	exit_routine();
}
