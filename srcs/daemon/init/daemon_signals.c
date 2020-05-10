/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 17:59:45 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

void	sigint_handle(int signo)
{
	(void)signo;
	g_denv->sigint = true;
}

void	init_signals(void)
{
	struct sigaction	sig_int;

	sig_int.sa_handler = sigint_handle;
	sig_int.sa_flags = 0;
	sigemptyset(&sig_int.sa_mask);
	sigaction(SIGINT, &sig_int, NULL);
}