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

t_env	*g_env;

int main(int ac, char **av)
{
	(void)ac;(void)av;
	t_env   env;

	ft_memset(&env, 0, sizeof(t_env));
	g_env = &env;

	//init logger
	if (init_log(&env) != 0)
		return (-1);

	// Do config file parsing

	// make network things
	if (make_socket(&env, DFLT_SOCKET) != 0)
		return (-1);
	if (bind_socket(&env) != 0)
		return (-1);

	//once all is ready, daemonize
	// And listen for incoming connections
	int connectionfd;
	int readstatus;
	int totalread = 0;
	char	buffer[1500];
	while (1)
	{
		if ((connectionfd = accept(env.unix_socket, NULL, NULL)) == -1)
		{
	  		perror("accept error");
	  		continue;
		}
		while ((readstatus = read(connectionfd, buffer, 1500)) > 0)
		{
	  		//printf("read %u bytes\n", readstatus);
			  totalread += readstatus;
		}
		printf("Read a total of %d bytes\n", totalread);
		if (readstatus == -1)
		{
	  		perror("read");
	  		break;
		}
		else if (readstatus == 0)
		{
	  		printf("Client disconnected\n");
	  		close(connectionfd);
			break;
		}
	}
	close(env.unix_socket);
	unlink(DFLT_SOCKET);

	return (0);
}
