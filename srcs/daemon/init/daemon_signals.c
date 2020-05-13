/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/13 17:24:38 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

void	sigall_handle(int signo)
{
	exit_routine(E_LOGLVL_CRIT, strsignal(signo));
}

void	init_signals(void)
{
	struct sigaction	sig_all;

	sigemptyset(&sig_all.sa_mask);
	sig_all.sa_handler = sigall_handle;
	sig_all.sa_flags = 0;
	sigaction(SIGTERM, &sig_all, NULL);
	sigaction(SIGQUIT, &sig_all, NULL);
	sigaction(SIGUSR1, &sig_all, NULL);
	sigaction(SIGUSR2, &sig_all, NULL);
	sigaction(SIGPIPE, &sig_all, NULL);
	sigaction(SIGINT, &sig_all, NULL);
}
