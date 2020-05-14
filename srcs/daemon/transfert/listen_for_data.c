/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listen_for_data.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 12:18:52 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 14:18:18 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void	client_connected(fd_set *master_set, const int connectionfd)
{
	tlog(E_LOGLVL_DEBG, "=> Client connecting...\n");
	if (g_denv->client_connected == 1)
	{
		tlog(E_LOGLVL_DEBG,
				"=> Client trying to connect, but no slot is avaible\n");
		close(connectionfd);
	}
	else
	{
		FD_SET(connectionfd, master_set);
		tlog(E_LOGLVL_DEBG, "=> Client connected\n");
		g_denv->client_connected = 1;
	}
}

static void	my_select(fd_set *recv_set)
{
	struct timeval	tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;
	if (select(DFL_FD_SETSIZE, recv_set, NULL, NULL, &tv) < 0)
	{
		if (errno != EINTR)
			exit_routine(E_LOGLVL_CRIT, strerror(errno));
	}
}

static void	fd_loop(fd_set *master_set, fd_set *recv_set)
{
	int				connectionfd;
	int				fd_nb;

	fd_nb = 0;
	while (fd_nb < DFL_FD_SETSIZE)
	{
		if (FD_ISSET(fd_nb, recv_set) != 0)
		{
			if (fd_nb == g_denv->unix_socket)
			{
				connectionfd = accept(g_denv->unix_socket, NULL, NULL);
				if (connectionfd == FAILURE)
					exit_routine(E_LOGLVL_CRIT, strerror(errno));
				else
					client_connected(master_set, connectionfd);
			}
			else
				handle_client_requests(fd_nb, master_set);
		}
		fd_nb++;
	}
}

void		listen_for_data(void)
{
	fd_set			recv_set;
	fd_set			master_set;

	FD_ZERO(&master_set);
	FD_ZERO(&recv_set);
	FD_SET(g_denv->unix_socket, &master_set);
	while (true)
	{
		recv_set = master_set;
		my_select(&recv_set);
		fd_loop(&master_set, &recv_set);
		waiter();
	}
}
