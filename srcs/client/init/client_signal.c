/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 22:15:47 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void    sigwinch_handle(int signo)
{
	(void)signo;
	g_env->sigwinch = 1;
}

void	sigint_handle(int signo)
{
	(void)signo;
	g_env->sigint = 1;
}

void	sigpipe_handle(int signo)
{
	(void)signo;
	g_env->sigpipe = 1;
}

void	sigcont_handle(int signo)
{
	(void)signo;
	apply_termmode(NEW);
	print_prompt(g_env);
	vct_print_fd(g_env->cur_line, STDERR_FILENO);
	calc_after_totalprint(g_env, g_env->cur_line);
}

void	init_signals(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_win;
	struct sigaction	sig_pipe;
	struct sigaction	sig_cont;

	sig_int.sa_handler = sigint_handle;
	sig_int.sa_flags = 0;
	sigemptyset(&sig_int.sa_mask);
	sigaction(SIGINT, &sig_int, NULL);
	sig_win.sa_handler = sigwinch_handle;
	sig_win.sa_flags = 0;
	sigemptyset(&sig_win.sa_mask);
	sigaction(SIGWINCH, &sig_win, NULL);
	sig_pipe.sa_handler = sigpipe_handle;
	sig_pipe.sa_flags = 0;
	sigemptyset(&sig_pipe.sa_mask);
	sigaction(SIGPIPE, &sig_pipe, NULL);
	sig_cont.sa_handler = sigcont_handle;
	sig_cont.sa_flags = SA_RESTART;
	sigemptyset(&sig_cont.sa_mask);
	sigaction(SIGCONT, &sig_cont, NULL);
}
