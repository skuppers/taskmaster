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
