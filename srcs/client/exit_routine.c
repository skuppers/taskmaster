/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/01 13:01:08 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void	exit_routine(void)
{
	if (g_env->cmd != NULL)
		ft_free_tab_str(g_env->cmd->av);
	release_termmode(g_env);
	vct_del(&g_env->opt.batch_cmd);
	free(g_env->orig);
	free(g_env->taskmst);
	exit(0);
}
