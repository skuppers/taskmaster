/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_connect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 19:05:55 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

int8_t		connect_to_daemon(t_env *env, char *socketpath)
{
	struct sockaddr_un addr;

	if ( (env->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
   		perror("socket error");
    	return (-1);
  	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, socketpath, sizeof(addr.sun_path)-1);
	
	if (connect(env->socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
   		printf("Error: Can't connect to taskmaster deamon.\n\n");
    	return (-1);
  	}
	return (0);
}
