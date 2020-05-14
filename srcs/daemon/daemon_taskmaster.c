/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 12:47:51 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_denv	*g_denv;
t_denv	*g_tmpenv;

int main(int ac, char **av, char **environ)
{
	t_denv   env;

	g_denv = &env;
	g_tmpenv = NULL;
	init(ac, av, environ);
	if (ft_strequ(env.opt.str[LOGLEVEL], LOGLVL_DEBG) == true
			&& (env.opt.optmask & OPT_NODAEMON))
		print_starting_debug(&env);
	daemonize();
	exit_routine(NO_MSG);
	return (EXIT_SUCCESS);
}
