/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 19:05:55 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

int32_t  make_socket(t_env *env, char *socketpath)
{
    int32_t 			fd;
	
    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        return (-1);
    }
	env->unix_socket = fd;
  	env->addr.sun_family = AF_UNIX;
	strncpy(env->addr.sun_path, socketpath, sizeof(env->addr.sun_path)-1);
    unlink(socketpath);
}

int8_t		bind_socket(t_env *env)
{
	if (bind(env->unix_socket, (struct sockaddr*)&env->addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind error");
		exit(-1);
	}

  	if (listen(env->unix_socket, 5) == -1)
	{
    	perror("listen error");
    	exit(-1);
	}
	return (0);
}

int main(int ac, char **av)
{
    t_env   env;

	ft_memset(&env, 0, sizeof(t_env));

	//init logger

	// Do config file parsing

	// make network things
	make_socket(&env, DFLT_SOCKET);
    bind_socket(&env);

	//once all is ready, daemonize
	// And listen for incoming connections
	return (0);
}