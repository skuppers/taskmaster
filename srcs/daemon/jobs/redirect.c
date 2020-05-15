/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 16:37:08 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/15 16:58:41 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void	get_pipes(t_instance *instance)
{
	int			fd[2];
	int			flag;
	int			i;

	i = 0;
	while (i < 6)
	{
		pipe(fd);
		instance->fd[i + (i != 0)] = fd[0];
		instance->fd[i + (i == 0)] = fd[1];
		flag = fcntl(instance->fd[i + (i == 0)], F_GETFL) | O_NONBLOCK;
		fcntl(instance->fd[i + (i == 0)] , F_SETFL, flag);
		i += 2;
	}
}

void	close_child_fd(t_instance *instance)
{
	close(instance->fd[CHILD_STDIN]);
	close(instance->fd[CHILD_STDOUT]);
	close(instance->fd[CHILD_STDERR]);
	instance->fd[CHILD_STDIN] = -1;
	instance->fd[CHILD_STDOUT] = -1;
	instance->fd[CHILD_STDERR] = -1;
}

void	close_parrent_fd(t_instance *instance)
{
	close(instance->fd[TRUE_STDIN]);
	close(instance->fd[TRUE_STDOUT]);
	close(instance->fd[TRUE_STDERR]);
	instance->fd[TRUE_STDIN] = -1;
	instance->fd[TRUE_STDOUT] = -1;
	instance->fd[TRUE_STDERR] = -1;
}
