/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctransfert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/13 13:15:39 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void			connect_to_daemon(const char *socketpath)
{
	struct sockaddr_un	addr;

	if ((g_env->unix_socket = socket(PF_UNIX, SOCK_STREAM, 0)) == FAILURE)
	{
		dprintf(STDERR_FILENO, "Error: %s\n", strerror(errno));
		return ;
	}
	ft_bzero(&addr, sizeof(addr));
	addr.sun_family = AF_UNIX;
	ft_strncpy(addr.sun_path, socketpath, sizeof(addr.sun_path) - 1);
	if (connect(g_env->unix_socket,
			(struct sockaddr*)&addr, sizeof(addr)) == FAILURE
			|| g_env->sigpipe == true)
	{
		g_env->sigpipe = false;
		dprintf(STDERR_FILENO, "Error: Can't connect to `%s': %s\n",
				socketpath, strerror(errno));
	}
	else
		dprintf(STDERR_FILENO, "Connected to %s\n", socketpath);
}

static t_vector	*read_feedback(const int fd)
{
	t_vector		*trame;

	trame = vct_new(DFL_VCT_SIZE);
	if (vct_creadline(trame, fd, EOT) > 0)
	{
		if (decode_feedback(trame) == NULL)
			vct_del(&trame);
		return (trame);
	}
	dprintf(STDERR_FILENO, "Daemon cannot be reached...\n");
	vct_del(&trame);
	return (NULL);
}

t_vector		*get_feedback(void)
{
	fd_set			recv_set;
	struct timeval	tv;
	int				fd;

	fd = -1;
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	FD_ZERO(&recv_set);
	FD_SET(g_env->unix_socket, &recv_set);
	if (select(DFL_FD_SETSIZE, &recv_set, NULL, NULL, &tv) < 0)
		dprintf(STDERR_FILENO, "select failed.\n");
	else
	{
		while (++fd < DFL_FD_SETSIZE)
		{
			if (FD_ISSET(fd, &recv_set) == 0)
				continue ;
			if (fd == g_env->unix_socket)
				return (read_feedback(fd));
		}
	}
	return (NULL);
}
