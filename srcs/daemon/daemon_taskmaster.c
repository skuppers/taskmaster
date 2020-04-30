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

int main(int ac, char **av)
{
	(void)ac;(void)av;
    t_env   env;

	ft_memset(&env, 0, sizeof(t_env));

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
	while (1)
		;

	close(env.unix_socket);
	unlink(DFLT_SOCKET);

	return (0);
}
