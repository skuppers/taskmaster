/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 14:34:36 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/01 15:33:55 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
# define COMMON_H

# include "libft.h"
# include "stdint.h"

#define DFL_FD_SETSIZE 16

/*
******************* Encode
*/

# define SOH	0x01	// Start Of Header
# define STX	0x02	// Start Of Text
# define ETX	0x03	// End Of Text
# define EOT	0x04	// End of Transmission
# define ENQ	0x05	// End of Line
# define US		0x1f	// Unit Separator

/*
**
** TRAME : SOH size (cmd + 128) [(ocp + 128)] [STX] [...] [US] [...] [ETX] ENQ (+ EOT)
**
*/

/*
******************* CMD
*/

# define NB_CMD			24
# define NO_OCP			0

enum	e_cmd_type
{
	ADD,
	AVAIL,
	CLEAR,
	EXIT,
	FG,
	HELP,
	MAINTAIL,
	OPEN,
	PID,
	QUIT,
	RELOAD,
	REMOVE,
	REREAD,
	RESTART,
	SHUTDOWN,
	SIGNAL,
	START,
	STATUS,
	STOP,
	TAIL,
	UPDATE,
	VERSION,
	SYN,
	ACK,
	BAD_CMD
};

typedef struct		s_cmd
{
	char			**av;
	enum e_cmd_type	type;
	int				ac;
	int				ocp;
	char			pad[4];
}					t_cmd;

#endif
