/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/13 18:25:27 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

void	sigall_handle(int signo)
{
	exit_routine(E_LOGLVL_INFO, strsignal(signo));
}

void	sighup_handle(int signo)
{
	tlog(E_LOGLVL_INFO, "%s\n", strsignal(signo));
	reread_file(NULL, NULL);
}

void	init_signals(void)
{
	struct sigaction	sig_all;
	struct sigaction	sig_hup;

	sigemptyset(&sig_all.sa_mask);
	sig_all.sa_handler = sigall_handle;
	sig_all.sa_flags = 0;
	sigaction(SIGTERM, &sig_all, NULL);
	sigaction(SIGQUIT, &sig_all, NULL);
	sigaction(SIGUSR1, &sig_all, NULL);
	sigaction(SIGUSR2, &sig_all, NULL);
	sigaction(SIGPIPE, &sig_all, NULL);
	sigaction(SIGINT, &sig_all, NULL);
	sigemptyset(&sig_hup.sa_mask);
	sig_hup.sa_handler = sighup_handle;
	sig_hup.sa_flags = SA_RESTART;
	sigaction(SIGHUP, &sig_hup, NULL);
}
