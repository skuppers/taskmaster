/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctransfert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 23:27:00 by ffoissey         ###   ########.fr       */
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
	int				ret;

	trame = vct_new(DFL_VCT_SIZE);
	if ((ret = vct_creadline(trame, fd, EOT)) > 0)
	{
		if (decode_feedback(trame) == NULL)
			vct_del(&trame);
		return (trame);
	}
	dprintf(STDERR_FILENO, "Daemon cannot be reached...\n");
	vct_del(&trame);
	return (NULL);
}

static int		get_waittime(void)
{
	if (g_env->cmd != NULL
			&& (g_env->cmd->type == START || g_env->cmd->type == STOP
				|| g_env->cmd->type == RESTART || g_env->cmd->type == UPDATE
				|| g_env->cmd->type == REMOVE))
		return (60);
	return (2);
}

t_vector		*get_feedback(void)
{
	fd_set			recv_set;
	struct timeval	tv;
	int				fd;

	fd = -1;
	tv.tv_sec = get_waittime();
	tv.tv_usec = 0;
	FD_ZERO(&recv_set);
	FD_SET(g_env->unix_socket, &recv_set);
	while (select(DFL_FD_SETSIZE, &recv_set, NULL, NULL, &tv) < 0)
	{
		dprintf(STDERR_FILENO, "select failed.\n");
		if (g_env->sigint == 0)
			return (NULL);
		else
			dprintf(STDERR_FILENO, "retry select...\n");
	}
	while (++fd < DFL_FD_SETSIZE)
	{
		if (FD_ISSET(fd, &recv_set) == 0)
			continue ;
		if (fd == g_env->unix_socket)
			return (read_feedback(fd));
	}
	return (NULL);
}
