/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transfert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 19:15:54 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 19:30:47 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

// 32,767 Bytes max message size
int8_t		sendall(int sockfd, const char *buf, uint16_t buflen)
{
    int32_t		bytes_written;
    int32_t		bytes_left;
    uint8_t		max_retries;

    bytes_written = 0;
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

int8_t      send_trame(int fd, t_vector *code, uint16_t len)
{
	int8_t		status;

	if ((status = sendall(fd, vct_getstr(code), len)) != 0)
	{
		if (status == -1)
			dprintf(STDERR_FILENO, "Fatal error while sending bytecode: %s\n", strerror(errno));
		else if (status == -2)
			dprintf(STDERR_FILENO, "Unable to send entire bytecode.\n");
		return (status);
	}
	dprintf(STDERR_FILENO, "Bytecode sent succesfully!\n");
/*	if (receive_ack() == FAILURE)
	{
		dprintf(STDERR_FILENO, "Did not receive message ACK\n");
	}*/
	return (0);
}
