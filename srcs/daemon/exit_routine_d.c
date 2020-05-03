/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_routine_d.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 17:07:35 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void	exit_routine(void)
{
    if (g_env->dict != NULL)
	    free_inifile(g_env->dict);
	close(g_env->unix_socket);
	unlink(DFL_SOCKET);
	ft_lstdel(&g_env->environ, free_env);
	ft_lstdel(&g_env->prgm_list, del_prgm);
	ft_lstdel(&g_env->group_list, del_group);
	exit(0);
}
