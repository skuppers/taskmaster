/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_connect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/02 17:54:18 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

int8_t		connect_to_daemon(t_env *env, char *soketpath)
{
	struct sockaddr_un addr;

	if ( (env->unix_socket = socket(PF_UNIX, SOCK_STREAM, 0)) == -1)
	{
   		perror("socket error");
    	return (-1);
  	}

	ft_bzero(&addr, sizeof(addr));
	addr.sun_family = AF_UNIX;
	ft_strncpy(addr.sun_path, socketpath, sizeof(addr.sun_path)-1);
	if (connect(env->unix_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
   		ft_dprintf(STDERR_FILENO,
				"Error: Can't connect to %s : %s\n\n", env->opt.str[SERVERURL],
			strerror(errno));
    	return (-1);
  	}
	  
	return (0);
}
