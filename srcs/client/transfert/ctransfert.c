/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctransfert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 12:34:04 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"

int8_t		connect_to_daemon(void)
{
	struct sockaddr_un addr;
	const char *socketpath = g_env->opt.str[SERVERURL];

	if ((g_env->unix_socket = socket(PF_UNIX, SOCK_STREAM, 0)) == FAILURE)
	{
   		perror("socket error");
    	return (FAILURE);
  	}
	ft_bzero(&addr, sizeof(addr));
	addr.sun_family = AF_UNIX;
	ft_strncpy(addr.sun_path, socketpath, sizeof(addr.sun_path)-1);
	if (connect(g_env->unix_socket,
			(struct sockaddr*)&addr, sizeof(addr)) == FAILURE
			|| g_env->sigpipe == true)
	{
		g_env->sigpipe = false;
   		ft_dprintf(STDERR_FILENO,
				"Error: Can't connect to `%s' : %s\n",
				g_env->opt.str[SERVERURL], strerror(errno));
    	return (FAILURE);
  	}
	dprintf(STDERR_FILENO, "Connected to %s\n", socketpath);
	return (SUCCESS);
}

t_vector		*get_feedback(void)
{
	t_vector		*vct;
	int				readbytes;
	fd_set			recv_set;
	struct timeval	tv;
	int				fd;

	fd = -1;
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	FD_ZERO (&recv_set);
	FD_SET (g_env->unix_socket, &recv_set);
	
	if (select(DFL_FD_SETSIZE, &recv_set, NULL, NULL, &tv) < 0)
	{
		dprintf(STDERR_FILENO, "select failed.\n");
		return (NULL);
	}
	while (++fd < DFL_FD_SETSIZE)
	{
		if (FD_ISSET(fd, &recv_set) == 0)
			continue ;
		if (fd == g_env->unix_socket)
		{
			vct = vct_new(DFL_VCT_SIZE);
			if ((readbytes = vct_creadline(vct, fd, EOT)) <= 0)
				dprintf(STDERR_FILENO, "Daemon cannot be reached...\n");
			else
				return (decode_feedback(vct));
			vct_del(&vct);
		}
	}
	return (NULL);
}
