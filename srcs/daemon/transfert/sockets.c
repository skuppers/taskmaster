/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/16 13:51:12 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void		make_socket(void)
{
	int	fd;

	if ((fd = socket(PF_UNIX, SOCK_STREAM, 0)) == FAILURE)
		exit_routine(E_LOGLVL_CRIT, strerror(errno));
	g_denv->unix_socket = fd;
	g_denv->addr.sun_family = AF_UNIX;
	strncpy(g_denv->addr.sun_path, g_denv->dfl_socket,
			sizeof(g_denv->addr.sun_path) - 1);
}

void		bind_socket(void)
{
	if (bind(g_denv->unix_socket,
			(struct sockaddr*)&g_denv->addr, sizeof(g_denv->addr)) == FAILURE)
	{
		if (errno != EADDRINUSE)
		{
			tlog(E_LOGLVL_CRIT, "Bind() failed . Is another daemon running?\n");
			exit_routine(E_LOGLVL_CRIT, strerror(errno));
		}
		tlog(E_LOGLVL_WARN, "Unlinking existing socket: %s\n",
				g_denv->addr.sun_path);
		unlink(g_denv->dfl_socket);
		if (bind(g_denv->unix_socket, (struct sockaddr*)&g_denv->addr,
				sizeof(g_denv->addr)) == FAILURE)
		{
			tlog(E_LOGLVL_CRIT, "Bind() failed . Is another daemon running?\n");
			exit_routine(E_LOGLVL_CRIT, strerror(errno));
		}
	}
	if (listen(g_denv->unix_socket, MAX_CLIENTS) == FAILURE)
	{
		tlog(E_LOGLVL_CRIT, "Listen() failed\n");
		exit_routine(E_LOGLVL_CRIT, strerror(errno));
	}
	tlog(E_LOGLVL_INFO, "Socket '%s' up and running.\n", g_denv->addr.sun_path);
}
