/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:31:47 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 19:04:18 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_open(t_cmd *cmd)
{
	if (cmd->ac == 0)
		ft_dprintf(STDERR_FILENO, "Error: Too few arguments\n");
	else if (ft_strnequ(cmd->av[0], UNIX_URI, UNIX_URI_SIZE) == false)
		ft_dprintf(STDERR_FILENO, "Error: Url must be unix://\n");
	else
	{
		close(g_env->unix_socket);
		connect_to_daemon(cmd->av[0] + UNIX_URI_SIZE);
	}
	return (NULL);
}
