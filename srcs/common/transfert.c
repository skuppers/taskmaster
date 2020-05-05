/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transfert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 19:15:54 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 21:10:43 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

// 32,767 Bytes max message size
int8_t      send_trame(int sockfd, t_vector *code, uint16_t buflen)
{
    int32_t		bytes_written;
    int32_t		bytes_left;
    uint8_t		max_retries;
	const char	*buf = vct_getstr(code);

    bytes_written = 0;
    max_retries = SEND_PARTIAL_RETRYS;          // Nombre de retry de send pour le cas de send partial
    while (buflen > 0 && max_retries-- > 0)     // Tant que (!tout envoyé) && (pas dépassé le nb de retry)
    {
        bytes_left = send(sockfd, buf + bytes_written, buflen, 0); // on envoi le buffer + offset de ce qui a déja été  envoyé
        if (bytes_left == FAILURE)
            return (FAILURE);
        bytes_written += bytes_left;
        buflen -= bytes_written;
    }
    if (max_retries == 0)
        return (UNCOMPLETE);							// -2 code d'erreur de retry
    return (SUCCESS);
}

static void	print_msg(t_print printer, uint8_t flag, uint8_t level, const char *msg)
{
	printer(flag == TO_LOG ? level : STDERR_FILENO, "%s\n", msg);
}

int8_t	try_to_send_trame(int fd, t_vector *trame, uint8_t flag, t_print f)
{
	int8_t			ret_send;
	char			*str;

	ret_send = send_trame(fd, trame, vct_len(trame));
	if (ret_send == FAILURE)
	{
		str = ft_asprintf("Fatal error while sending bytecode: %s",
					strerror(errno));
		print_msg(f, flag, E_LOGLVL_ERRO, str);
		ft_strdel(&str);
	}
	else if (ret_send == UNCOMPLETE)
		print_msg(f, flag, E_LOGLVL_ERRO, "Unable to send entire bytecode");
	else
		print_msg(f, flag, E_LOGLVL_INFO, "Bytecode sent succesfully !");
	return (ret_send);
}
