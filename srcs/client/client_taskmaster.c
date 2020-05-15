/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/15 19:23:28 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_env		*g_env;

int		main(int ac, char **av)
{
	t_env	environment;

	g_env = &environment;
	init(ac, av);
	if (environment.opt.mask & OPT_BATCHCMD)
		routine(environment.opt.batch_cmd, DEL_FEEDBACK);
	if (isatty(STDERR_FILENO) == false)
		canonic_mode(true);
	if (environment.opt.mask & OPT_INTERACTIVE)
		read_cmd();
	exit_routine(NO_MSG);
	return (EXIT_SUCCESS);
}
