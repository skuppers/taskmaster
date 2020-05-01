/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sockets.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 19:05:55 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

int8_t  make_socket(t_env *env, char *socketpath)
{
    int32_t 			fd;
	
    if ((fd = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		taskmaster_fatal("socket", "Failed to create socket");
		print_log(env, LOG_CRIT, "Socket failed - %s\n", strerror(errno));
        return (-1);
    }
	env->unix_socket = fd;
  	env->addr.sun_family = AF_UNIX;
	strncpy(env->addr.sun_path, socketpath, sizeof(env->addr.sun_path) - 1);
	return (0);
}

int8_t		bind_socket(t_env *env)
{
	if (bind(env->unix_socket, (struct sockaddr*)&env->addr, sizeof(env->addr)) == -1)
	{
		taskmaster_fatal("bind", "Failed to bind socket");
		print_log(env, LOG_CRIT, "Bind failed - %s\n", strerror(errno));
        return (-1);
	}
  	if (listen(env->unix_socket, MAX_CLIENTS) == -1)
	{
    	taskmaster_fatal("listen", "Failed to mark socket as passive");
		print_log(env, LOG_CRIT, "Listen failed - %s\n", strerror(errno));
        return (-1);
	}
	return (0);
}
