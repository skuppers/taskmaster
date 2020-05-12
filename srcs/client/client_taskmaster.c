/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/11 17:00:54 by ffoissey         ###   ########.fr       */
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
	if ((g_env->opt.mask & OPT_INTERACTIVE) && isatty(STDIN_FILENO) == false)
		exit_routine(ERR, "Not a tty");
	if (environment.opt.mask & OPT_INTERACTIVE)
		read_cmd();
	exit_routine(NO_MSG);
	return (EXIT_SUCCESS);
}
