/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_routine_d.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 21:21:27 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void	exit_routine(void)
{
    if (g_denv->dict != NULL)
	    free_inifile(g_denv->dict);
	close(g_denv->unix_socket);
	unlink(DFL_SOCKET);
	ft_lstdel(&g_denv->environ, free_env);
	ft_lstdel(&g_denv->prgm_list, del_prgm);
	exit(0);
}
