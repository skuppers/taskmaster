/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termmode.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 11:12:07 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void		create_termmode(void)
{
	struct termios	term;

	g_env->orig = (struct termios *)malloc(sizeof(struct termios));
	if (tcgetattr(STDIN_FILENO, &term) == SUCCESS)
	{
		memcpy(g_env->orig, &term, sizeof(struct termios));
		term.c_lflag &= ~(ICANON);
		term.c_lflag &= ~(ECHO);
		term.c_lflag |= ISIG;
		term.c_cc[VMIN] = 1;
		term.c_cc[VTIME] = 0;
		g_env->taskmst = (struct termios *)malloc(sizeof(struct termios));
		if (g_env->taskmst != NULL)
			memcpy(g_env->taskmst, &term, sizeof(struct termios));
	}
	if (g_env->taskmst == NULL || g_env->orig == NULL)
		exit_routine(ERR, "Failed to set termmode");
}

void		apply_termmode(const uint8_t flag)
{
	if (g_env->orig != NULL && g_env->taskmst != NULL)
	{
		if (tcsetattr(STDIN_FILENO, TCSADRAIN,
				(flag & NEW) ? g_env->taskmst : g_env->orig) == FAILURE)
			exit_routine(ERR, "Failed to apply termmode");
	}
	else
		exit_routine(ERR, "termmode: unexpected error");
}

void		canonic_mode(const bool flag)
{
	struct sigaction	sig_win;

	sig_win.sa_flags = 0;
	sigemptyset(&sig_win.sa_mask);
	if (flag == true)
	{
		g_env->taskmst->c_lflag |= ICANON;
		g_env->taskmst->c_lflag |= ECHO;
		sig_win.sa_handler = SIG_DFL;
	}
	else
	{
		g_env->taskmst->c_lflag &= ~ICANON;
		g_env->taskmst->c_lflag &= ~ECHO;
		sig_win.sa_handler = sigwinch_handle;
	}
	sigaction(SIGWINCH, &sig_win, NULL);
	apply_termmode(NEW);
}
