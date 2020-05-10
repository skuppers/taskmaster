/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 12:19:21 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 12:55:18 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static void	init_readline(void)
{
	char	*value;

	if ((value = getenv("TERM")) == NULL)
		exit_routine(ERR, "Please specify the TERM variable in your shell.");
	else if (ft_strequ(value, "xterm") == FALSE
			&& ft_strequ(value, "xterm-256color") == FALSE)
		exit_routine(ERR, "Please set yout TERM variable to 'xterm'"
			" or 'xterm-256color.");
	create_termmode();
	apply_termmode(NEW);
	assign_keycodes();
	link_keys_functions(g_env->actionkeys);
}

void		init(int ac, char **av)
{
	ft_bzero(g_env, sizeof(t_env));
	get_opt(ac - 1, av + 1);
	init_signals();
	connect_to_daemon(g_env->opt.str[SERVERURL]);
	init_readline();
}
