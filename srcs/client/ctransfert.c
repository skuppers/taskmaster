/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transfert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 15:43:01 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"
/*
void	select_error(void)
{
	printf("Select() errror\n");
	exit_routine();
}

int8_t		receive_ack(t_env *env)
{
	struct timeval	tv;
	fd_set			recv_set;


	tv.tv_sec = 3;
	tv.tv_usec = 0;
	FD_SET (env->unix_socket, &recv_set);

	if (select(DFL_FD_SETSIZE, &recv_set, NULL, NULL, &tv) < 0)
		select_error();

	int32_t		readstatus;
	t_vector	*vct;
	int fd_nb = -1;
	while (++fd_nb < DFL_FD_SETSIZE)
	{
		if (FD_ISSET(fd_nb, &recv_set) != 0)
		{
			if (fd_nb == env->unix_socket)
			{
				vct = vct_new(DFL_VCT_SIZE);
				if ((readstatus = vct_creadline(vct, env->unix_socket, EOT)) <= 0)
				{
					printf("No data / Error\n");
					return (-1);
				}
				else 
				{
					printf("ACK\n");
					return (0);
				}	
			}
		}
	}
	return (-1);
}*/

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

int8_t      send_bytecode(t_vector *code, uint16_t len)
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
	t_vector *bytecode;
	
	bytecode = generate_bytecode(get_cmd_struct(VERSION, NULL), 0);
//	debug_print_bytecode(bytecode);
	if (send_bytecode(bytecode, (uint16_t)vct_len(bytecode)) != 0)
	{
		env->sigpipe = 0;
		vct_del(&bytecode);
		return (-1);
	}
	vct_del(&bytecode);
	return (0);
}

int8_t		request_daemon(t_vector *bytecode, size_t codelen)
{
	(void)codelen;
	if (check_connection(g_env) < 0)
	{
		dprintf(STDERR_FILENO, "Connection is not alive: broken pipe\n");
		return (-1);
	}
	send_bytecode(bytecode, (uint16_t)vct_len(bytecode));
	return (0);
}
