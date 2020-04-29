/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_taskmaster.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:36:21 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 13:18:34 by ffoissey         ###   ########.fr       */
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

typedef	t_cmd	*(*t_builtin)(t_vector *);

/*
**** BUILT_IN
*/

t_cmd	*blt_add(t_vector *arg);
t_cmd	*blt_avail(t_vector *arg);
t_cmd	*blt_clear(t_vector *arg);
t_cmd	*blt_exit(t_vector *arg);
t_cmd	*blt_fg(t_vector *arg);
t_cmd	*blt_maintail(t_vector *arg);
t_cmd	*blt_open(t_vector *arg);
t_cmd	*blt_pid(t_vector *arg);
t_cmd	*blt_quit(t_vector *arg);
t_cmd	*blt_reload(t_vector *arg);
t_cmd	*blt_remove(t_vector *arg);
t_cmd	*blt_reread(t_vector *arg);
t_cmd	*blt_restart(t_vector *arg);
t_cmd	*blt_shutdown(t_vector *arg);
t_cmd	*blt_signal(t_vector *arg);
t_cmd	*blt_start(t_vector *arg);
t_cmd	*blt_status(t_vector *arg);
t_cmd	*blt_stop(t_vector *arg);
t_cmd	*blt_tail(t_vector *arg);
t_cmd	*blt_update(t_vector *arg);
t_cmd	*blt_version(t_vector *arg);

#endif
