/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 15:43:01 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"

void    update_winsize(int signo)
{
    (void)signo;
    g_env->sigwinch = 1;
}  

void	sigint_handle(int signo)
{
	(void)signo;
	g_env->sigint = 1;
}

void					init_signals(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_win;

	sig_int.sa_handler = sigint_handle;
	sig_int.sa_flags = 0;
	sigemptyset(&sig_int.sa_mask);
	sigaction(SIGINT, &sig_int, NULL);
	sig_win.sa_handler = update_winsize;
	sig_win.sa_flags = 0;
	sigemptyset(&sig_win.sa_mask);
	sigaction(SIGWINCH, &sig_win, NULL);
}
