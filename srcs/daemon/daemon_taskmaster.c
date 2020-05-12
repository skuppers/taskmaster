/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 18:28:22 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_denv	*g_denv;
t_denv	*g_newenv;

int main(int ac, char **av, char **environ)
{
	t_denv   env;
	t_denv   newenv;

	g_denv = &env;
	g_newenv = &newenv;
	init(ac, av, environ);
	if (ft_strequ(env.opt.str[LOGLEVEL], LOGLVL_DEBG) == true)
		print_starting_debug(&env);
	daemonize();
	exit_routine(NO_MSG);
	return (EXIT_SUCCESS);
}
