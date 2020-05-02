/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:31:47 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/02 17:53:11 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_open(t_cmd *cmd)
{
	struct sockaddr_un addr;

	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: url must be http:// or unix://\n");
		return (NULL);
	}
	ft_bzero(&addr, sizeof(addr));
	addr.sun_family = AF_UNIX;
	ft_strncpy(addr.sun_path, cmd->av[1], sizeof(addr.sun_path)-1);
	if (connect(g_env->unix_socket,
			(struct sockaddr*)&addr, sizeof(addr)) == FAILURE)
	{
   		ft_dprintf(STDERR_FILENO,
				"Error: Can't connect to %s : %s\n\n", cmd->av[0])
  	}
	return (NULL);
}
