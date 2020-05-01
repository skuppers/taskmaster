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

	if ( (env->unix_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
   		perror("socket error");
    	return (-1);
  	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, socketpath, sizeof(addr.sun_path)-1);
	if (connect(env->unix_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
   		printf("Error: Can't connect to unix://%s\n\n", DFLT_SOCKET);
    	return (-1);
  	}
	return (0);
}
