/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 19:05:55 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

// 32,767 Bytes max message size
int16_t sendall(int sockfd, const char *buf, int16_t buflen)
{
    int16_t     bytes_written;
    int16_t     bytes_left;
    uint8_t     max_retries;

    bytes_written = 0;
    bytes_left = 0;
    max_retries = SEND_PARTIAL_RETRYS;
    while (buflen > 0 && max_retries-- > 0)
    {
        bytes_left = send(sockfd, buf + bytes_written, buflen, 0);
        if (bytes_left == -1)
            return (-1);
        bytes_written += bytes_left;
        buflen -= bytes_written;
    }
    if (max_retries == 0)
        return (-1);
    return (bytes_written);
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
		return (-1);
	}
	printf("Bytecode sent succesfully!\n");
	return (0);
}
