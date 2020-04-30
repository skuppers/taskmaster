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
