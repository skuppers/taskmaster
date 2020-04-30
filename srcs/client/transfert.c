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

// 32,767 Bytes max message size
int8_t		sendall(int sockfd, const char *buf, uint16_t buflen)
{
    int32_t	bytes_written;
    int32_t	bytes_left;
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

	if ((status = sendall(g_env->socket_fd, vct_getstr(code), len)) != 0)
	{
		if (status == -1)
			printf("Fatal error while sending bytecode: %s\n", strerror(errno));
		else if (status == -2)
			printf("Unable to send entire bytecode.\n");
		return (-1);
	}
	printf("Bytecode sent succesfully!\n");
	return (0);
}
