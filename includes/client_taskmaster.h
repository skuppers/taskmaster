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

# include <termios.h>
# include <unistd.h>
# include "libft.h"
# include "stdint.h"

/*****************	Readline **************/

# define AK_AMOUNT					14

# define AK_ARROW_UP_MASK			0x1b5b410000000000
# define AK_ARROW_DOWN_MASK 		0x1b5b420000000000
# define AK_ARROW_RIGHT_MASK		0x1b5b430000000000
# define AK_ARROW_LEFT_MASK 		0x1b5b440000000000
# define AK_HOME_MASK 				0x1b5b480000000000
# define AK_END_MASK 				0x1b5b460000000000
# define AK_CTRL_A_MASK 			0x0100000000000000
# define AK_CTRL_E_MASK		   		0x0500000000000000
# define AK_DELETE_MASK		   		0x1b5b337e00000000
# define AK_BACKSPACE_MASK		 	0x7f00000000000000
# define AK_CTRL_D_MASK 			0x0400000000000000
# define AK_CTRL_L_MASK				0x0c00000000000000
# define AK_CTRL_R_MASK				0x1200000000000000
# define AK_TAB_MASK				0x0900000000000000

enum	e_action_keys
{
	AK_ARROW_UP,
	AK_ARROW_DOWN,
	AK_ARROW_RIGHT,
	AK_ARROW_LEFT,
	AK_HOME,
	AK_END,
	AK_CTRL_A,
	AK_CTRL_E,
	AK_BACKSPACE,
	AK_DELETE,
	AK_CTRL_D,
	AK_CTRL_L,
	AK_CTRL_R,
	AK_TABULATION,
};

/* ************************************* */

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




typedef struct		s_env
{
	struct termios	*orig;
	struct termios	*taskmst;
	uint64_t		ak_masks[AK_AMOUNT];
	int8_t			(*actionkeys[AK_AMOUNT])(struct s_env *env, t_vector *vct);

}					t_env;

typedef int8_t		(*t_actionkeys)(t_env *env, t_vector *vct);

void				create_termmode(t_env *environment);
uint8_t    			set_termmode(t_env *environment);
void    			release_termmode(t_env *environment);
int					tsk_readline(t_vector *vct, const int fd, t_env *env);
uint64_t			assign_keycodes(t_env *env);
uint64_t			link_keys_functions(t_actionkeys actionkeys[AK_AMOUNT]);


/*********************** ACTION KEYS ********************/

int8_t		ak_arrow_up(t_env *env, t_vector *vct);
int8_t		ak_arrow_down(t_env *env, t_vector *vct);
int8_t		ak_arrow_left(t_env *env, t_vector *vct);
int8_t		ak_arrow_right(t_env *env, t_vector *vct);

int8_t		ak_home(t_env *env, t_vector *vct);
int8_t		ak_end(t_env *env, t_vector *vct);
int8_t		ak_delete(t_env *env, t_vector *vct);
int8_t		ak_backspace(t_env *env, t_vector *vct);
/*
int8_t		ak_ctrl_d(t_registry *shell, t_sle *sle);
int8_t		ak_ctrl_l(t_registry *shell, t_sle *sle);

int8_t		ak_ctrl_r(t_registry *shell, t_sle *sle);

int8_t		ak_hightab(t_registry *shell, t_sle *sle);
int8_t		ak_ctrl_t(t_registry *shell, t_sle *sle);
*/

#endif
