/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_routine_d.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 16:41:57 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void	exit_routine(const int flag, ...)
{
	va_list	arg;
	char	*err_str;

    if (g_denv->dict != NULL)
	    free_inifile(g_denv->dict);
	close(g_denv->unix_socket);
	unlink(DFL_SOCKET);
	ft_lstdel(&g_denv->environ, free_env);
	ft_lstdel(&g_denv->prgm_list, del_prgm);
	flock(g_denv->lock, LOCK_UN);
	if (flag != NO_MSG)
	{
		va_start(arg, flag);
		err_str = va_arg(arg, char *);
		if (err_str != NULL)
			tlog(flag, "ERROR: %s: %s\n", err_str);
		va_end(arg);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
