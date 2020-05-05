/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctransfert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 12:35:28 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"

// 32,767 Bytes max message size
int8_t		sendall(int sockfd, const char *buf, uint16_t buflen)
{
    int32_t		bytes_written;
    int32_t		bytes_left;
    uint8_t		max_retries;

    bytes_written = 0;
    bytes_left = 0;
    max_retries = SEND_PARTIAL_RETRYS;          // Nombre de retry de send pour le cas de send partial
    while (buflen > 0 && max_retries-- > 0)     // Tant que (!tout envoyé) && (pas dépassé le nb de retry)
    {
        bytes_left = send(sockfd, buf + bytes_written, buflen, 0); // on envoi le buffer + offset de ce qui a déja été  envoyé
        if (bytes_left == -1)
            return (-1);
        bytes_written += bytes_left;
        buflen -= bytes_written;
    }
    if (max_retries == 0)
        return (-2);							// -2 code d'erreur de retry
    return (0);
}

int8_t      send_trame(t_vector *code, uint16_t len)
{
	int8_t		status;

	if ((status = sendall(g_env->unix_socket, vct_getstr(code), len)) != 0)
	{
		if (status == -1)
			printf("Fatal error while sending bytecode: %s\n", strerror(errno));
		else if (status == -2)
			printf("Unable to send entire bytecode.\n");
		return (status);
	}
	printf("Bytecode sent succesfully!\n");
/*	if (receive_ack() == FAILURE)
	{
		dprintf(STDERR_FILENO, "Did not receive message ACK\n");
	}*/
	return (0);
}

int8_t		check_connection(t_env *env)
{
	t_vector *trame;
	
	trame = generate_bytecode(get_cmd_struct(VERSION, NULL), 0);
//	debug_print_bytecode(trame);
	if (send_trame(trame, (uint16_t)vct_len(trame)) != 0)
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
	readbytes = 0;
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
