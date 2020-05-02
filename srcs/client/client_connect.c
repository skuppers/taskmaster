/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_connect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/02 18:25:29 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

int8_t		connect_to_daemon(t_env *env, char *socketpath)
{
	struct sockaddr_un addr;

	if ((env->unix_socket = socket(PF_UNIX, SOCK_STREAM, 0)) == FAILURE)
	{
   		perror("socket error");
    	return (FAILURE);
  	}
	ft_bzero(&addr, sizeof(addr));
	addr.sun_family = AF_UNIX;
	ft_strncpy(addr.sun_path, socketpath, sizeof(addr.sun_path)-1);
	if (connect(env->unix_socket,
			(struct sockaddr*)&addr, sizeof(addr)) == FAILURE)
	{
   		ft_dprintf(STDERR_FILENO,
				"Error: Can't connect to `%s' : %s\n", env->opt.str[SERVERURL],
			strerror(errno));
    	return (FAILURE);
  	}
	  
	return (SUCCESS);
}
