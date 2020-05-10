/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 14:34:36 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 13:47:26 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMON_H
# define COMMON_H

# include "libft.h"
# include <stdint.h>
# include <stdbool.h>
# include <errno.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/select.h>
# include <sys/socket.h>

/*
**
**					---	---	TRAME SEQUENCE --- ---
** SOH size (cmd + 128) [(ocp + 128)] [STX arg [US] [...]] ETX] ENQ (+ EOT)
** ``` ```` ```````````                                         ``` ```````
**
** SOH  -> Start Of Header
** STX  -> Start Of Text
** ETX  -> End Of Text
** EOT  -> End of Transmission
** ENQ  -> End of Line
** US   -> Unit Separator
** size -> size of trame without EOT
**
*/

#define DFL_FD_SETSIZE	16

/*
******************* Encode
*/

# define SOH	0x01
# define STX	0x02
# define ETX	0x03
# define EOT	0x04
# define ENQ	0x05
# define US		0x1f

# define SEND_RETRYS				3
# define SEND_PARTIAL_RETRYS		5
# define UNCOMPLETE					-2

/*
******************* CMD
*/

# define NO_ARG			0
# define ONE_ARG		1
# define TAB_ARG		2

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

void		debug_print_bytecode(t_vector *bytecode);
const char *get_keyword(const uint8_t i);
void		debug_cmd(t_cmd *cmd);
t_cmd		*decode_cmd(t_vector *trame);
t_cmd		*get_cmd_struct(enum e_cmd_type type, t_vector *arg);
t_vector	*generate_bytecode(t_cmd *cmd, int ocp);
t_vector	*generate_feedback(t_vector *input);
t_vector	*decode_feedback(t_vector *trame);


/********************** LOGGER ********************/

# define TO_LOG						0
# define TO_PRINT					1

typedef	int	(*t_print)(int, const char *, ...);

int8_t		try_to_send_trame(const int fd, t_vector *trame, const uint8_t flag,
				t_print f);

enum	e_loglvl
{
	E_LOGLVL_DEBG,
	E_LOGLVL_INFO,
	E_LOGLVL_WARN,
	E_LOGLVL_ERRO,
	E_LOGLVL_CRIT
};

int8_t		init_log(void);
int			tlog(int loglvl, const char *message, ...);

# define TIMEBUFFERSZ			64

# define LOG_DEBG_STR		"[DEBUG]    "
# define LOG_INFO_STR		"[INFO]     "
# define LOG_WARN_STR		"[WARNING]  "
# define LOG_ERRO_STR		"[ERROR]    "
# define LOG_CRIT_STR		"[CRITICAL] "

# define LOGLVL_DEBG		"debug"
# define LOGLVL_INFO		"info"
# define LOGLVL_WARN		"warning"
# define LOGLVL_ERRO		"error"
# define LOGLVL_CRIT		"critical"

#endif
