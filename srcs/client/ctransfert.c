/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctransfert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 19:23:02 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"

int8_t		check_connection(t_env *env)
{
	t_vector *trame;
	
	trame = generate_bytecode(get_cmd_struct(VERSION, NULL), 0);
//	debug_print_bytecode(trame);
	if (send_trame(g_env->unix_socket, trame, (uint16_t)vct_len(trame)) != SUCCESS)
	{
		env->sigpipe = 0;
		vct_del(&trame);
		return (FAILURE);
	}
	vct_del(&trame);
	return (SUCCESS);
}

t_vector		*get_feedback(t_env *env)
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
	FD_SET (env->unix_socket, &recv_set);
	
	if (select(DFL_FD_SETSIZE, &recv_set, NULL, NULL, &tv) < 0)
	{
		dprintf(STDERR_FILENO, "select failed.\n");
		return (NULL);
	}
	while (++fd < DFL_FD_SETSIZE)
	{
		if (FD_ISSET(fd, &recv_set) == 0)
			continue ;
		if (fd == env->unix_socket)
		{
			vct = vct_new(DFL_VCT_SIZE);
			if ((readbytes = vct_creadline(vct, fd, EOT)) <= 0)
				dprintf(STDERR_FILENO, "Daemon cannot be reached...\n");
			else
				return (decode_feedback(vct));
			vct_del(&vct);
		}
	}
	dprintf(STDERR_FILENO, "passed while\n");
	return (NULL);
}
