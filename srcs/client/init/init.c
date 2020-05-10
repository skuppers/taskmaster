/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 12:19:21 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 12:36:37 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"

static void	init_readline(void)
{
	create_termmode();
	apply_termmode(NEW);
	assign_keycodes();
	link_keys_functions(g_env->actionkeys);
}

void init(int ac, char **av)
{
	ft_bzero(g_env, sizeof(t_env));
	get_opt(ac - 1, av + 1);
	init_signals();
	connect_to_daemon();
	init_readline();
}
