/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_taskmaster.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:36:21 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 12:11:49 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_TASKMASTER_H
# define CLIENT_TASKMASTER_H

# include "libft.h"
# include "stdint.h"

# define NB_CMD			21

# define PROMPT			"taskmaster> "

/*
********* ERROR MANAGEMENT
*/

# define ERR_BAD_CMD	"Bad Cmd"


enum	e_cmd_type
{
	ADD,
	AVAIL,
	CLEAR,
	EXIT,
	FG,
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
	BAD_CMD
};

typedef struct		s_cmd
{
	t_list			*arg;
	enum e_cmd_type	type;
	int				ac;
}					t_cmd;

#endif
