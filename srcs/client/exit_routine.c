/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 10:59:44 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void	exit_routine(int flag, ...)
{
	va_list	arg;

	if (g_env->cmd != NULL)
		ft_free_tab_str(g_env->cmd->av);
	iniparser_freedict(g_env->dict);
	vct_del(&g_env->opt.batch_cmd);
	if (g_env->orig != NULL && g_env->taskmst != NULL)
		apply_termmode(RELEASE);
	free(g_env->orig);
	free(g_env->taskmst);
	if (flag & EXIT)
		dprintf(STDERR_FILENO, "\nexit\n");
	else if (flag & ERR)
	{
		va_start(arg, flag);
		dprintf(STDERR_FILENO, "ERROR: %s\n", va_arg(arg, char *));
		va_end(arg);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
