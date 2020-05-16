/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctransfert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/16 12:59:44 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void					restart_connection(void)
{
	int		i;

	i = RESTART_TIME;
	close(g_env->unix_socket);
	canonic_mode(true);
	dprintf(2, "Reconnection in %d ", RESTART_TIME);
	while (i > 0)
	{
		sleep(1);
		i--;
		dprintf(2, "%d%c", i, (i == 0) ? '\n' : ' ');
	}
	connect_to_daemon(g_env->opt.str[SERVERURL]);
	canonic_mode(false);
}

void					connect_to_daemon(const char *socketpath)
{
	struct sockaddr_un	addr;

	if ((g_env->unix_socket = socket(PF_UNIX,
				SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0)) == FAILURE)
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

static t_vector			*read_feedback(const int fd)
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

static struct timeval	get_waittime(void)
{
	struct timeval	tv;

	if (g_env->cmd != NULL
			&& (g_env->cmd->type == START || g_env->cmd->type == STOP
				|| g_env->cmd->type == RESTART || g_env->cmd->type == UPDATE
				|| g_env->cmd->type == REMOVE))
	{
		tv.tv_sec = 60;
		tv.tv_usec = 0;
	}
	else
	{
		tv.tv_sec = 0;
		tv.tv_usec = 250000;
	}
	return (tv);
}

t_vector				*get_feedback(void)
{
	fd_set			recv_set;
	struct timeval	tv;
	int				fd;

	fd = -1;
	tv = get_waittime();
	FD_ZERO(&recv_set);
	FD_SET(g_env->unix_socket, &recv_set);
	while (select(DFL_FD_SETSIZE, &recv_set, NULL, NULL, &tv) < 0)
	{
		dprintf(STDERR_FILENO, "select failed.\n");
		if (g_env->sigint == 0)
			return (NULL);
		dprintf(STDERR_FILENO, "retry select...\n");
		g_env->sigint = false;
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
