/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 20:45:14 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

int8_t  make_socket(t_denv *env, char *socketpath)
{
    int32_t 			fd;
	
    if ((fd = socket(PF_UNIX, SOCK_STREAM, 0)) == FAILURE)
	{
		tlog(E_LOGLVL_CRIT, "Socket failed - %s\n", strerror(errno));
        exit_routine();
    }
	env->unix_socket = fd;
  	env->addr.sun_family = AF_UNIX;
	strncpy(env->addr.sun_path, socketpath, sizeof(env->addr.sun_path) - 1);
	return (SUCCESS);
}

int8_t		bind_socket(t_denv *env)
{
	if (bind(env->unix_socket, (struct sockaddr*)&env->addr, sizeof(env->addr)) == FAILURE)
	{
		if (errno != EADDRINUSE)
		{
			tlog(E_LOGLVL_CRIT, "Bind() failed - %s. Is another daemon running?\n", strerror(errno));
        	exit_routine();
		}
		tlog(E_LOGLVL_WARN, "Unlinking existing socket: %s\n", env->addr.sun_path);
		unlink(DFL_SOCKET);
		if (bind(env->unix_socket, (struct sockaddr*)&env->addr, sizeof(env->addr)) == FAILURE)
		{
			tlog(E_LOGLVL_CRIT, "Bind() failed - %s. Is another daemon running?\n", strerror(errno));
        	exit_routine();
		}
	}
  	if (listen(env->unix_socket, MAX_CLIENTS) == -1)
	{
		tlog(E_LOGLVL_CRIT, "Listen() failed - %s\n", strerror(errno));
        exit_routine();
	}
	tlog(E_LOGLVL_INFO, "Socket up and running.\n");
	return (SUCCESS);
}
