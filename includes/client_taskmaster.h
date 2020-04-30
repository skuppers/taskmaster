/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_taskmaster.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:36:21 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 21:01:35 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_TASKMASTER_H
# define CLIENT_TASKMASTER_H

# include <termios.h>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/un.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <errno.h>
# include "libft.h"
# include "stdint.h"

/****************   General   ****************/

# define PROMPT			"taskmaster> "

/**************** Networking *****************/

# define DFLT_SOCKET				"/tmp/tsock"
# define SEND_RETRYS				3
# define SEND_PARTIAL_RETRYS		5


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
	AK_BACKSPACE,
	AK_DELETE,
	AK_CTRL_A,
	AK_CTRL_E,
	AK_CTRL_D,
	AK_CTRL_L,
	AK_CTRL_R,
	AK_TABULATION,
};
/***********************************************************/

# define NB_CMD			22
# define NO_OCP			-1

# define NO_ARG			0
# define ONE_ARG		1
# define TAB_ARG		2

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
	BAD_CMD
};

typedef struct		s_cmd
{
	char			**av;
	enum e_cmd_type	type;
	int				ac;
}					t_cmd;


typedef struct		s_env
{
	struct termios			*orig;
	struct termios			*taskmst;

	uint64_t				ak_masks[AK_AMOUNT];
	int8_t					(*actionkeys[AK_AMOUNT])
						(struct s_env *env, t_vector *vct, char c[BUFF_SIZE]);

	uint32_t				cursoridx;
	uint16_t				cursorx;
	uint16_t				cursory;
	uint16_t				winwid;
	uint16_t				winhei;

	volatile sig_atomic_t	sigint;
	volatile sig_atomic_t	sigwinch;

	int32_t					socket_fd;
	
	t_cmd					*cmd;


}					t_env;

extern t_env		*g_env;

typedef int8_t		(*t_actionkeys)(t_env *env, t_vector *vct, char c[BUFF_SIZE]);

void				create_termmode(t_env *environment);
uint8_t    			set_termmode(t_env *environment);
void    			release_termmode(t_env *environment);
int					tsk_readline(t_vector *vct, const int fd, t_env *env);
uint64_t			assign_keycodes(t_env *env);
uint64_t			link_keys_functions(t_actionkeys actionkeys[AK_AMOUNT]);

void				exit_routine(void);

int8_t				connect_to_daemon(t_env *env, char *socketname);

void				init_signals(void);

int8_t      		send_bytecode(t_vector *code, uint16_t len);

/*********************** ACTION KEYS ********************/

int8_t		ak_arrow_up(t_env *env, t_vector *vct, char c[BUFF_SIZE]);
int8_t		ak_arrow_down(t_env *env, t_vector *vct, char c[BUFF_SIZE]);
int8_t		ak_arrow_left(t_env *env, t_vector *vct, char c[BUFF_SIZE]);
int8_t		ak_arrow_right(t_env *env, t_vector *vct, char c[BUFF_SIZE]);
int8_t		ak_home(t_env *env, t_vector *vct, char c[BUFF_SIZE]);
int8_t		ak_end(t_env *env, t_vector *vct, char c[BUFF_SIZE]);
int8_t		ak_delete(t_env *env, t_vector *vct, char c[BUFF_SIZE]);
int8_t		ak_backspace(t_env *env, t_vector *vct, char c[BUFF_SIZE]);
int8_t		ak_ctrl_d(t_env *env, t_vector *vct, char c[BUFF_SIZE]);
int8_t		ak_ctrl_l(t_env *env, t_vector *vct, char c[BUFF_SIZE]);
int8_t		ak_ctrl_r(t_env *env, t_vector *vct, char c[BUFF_SIZE]);
int8_t		ak_hightab(t_env *env, t_vector *vct, char c[BUFF_SIZE]);

typedef	t_vector	*(*t_builtin)(t_cmd *);
typedef	void		(*t_help)(void);


/************************ Parser *****************/

int			parser(t_vector *line);
const char *get_keyword(const uint8_t i);


/*
** Encode
*/

# define SOH	0x01	// Start Of Header
# define STX	0x02	// Start Of Text
# define ETX	0x03	// End Of Text
# define ENQ	0x05	// End of Line
# define US		0x1f	// Unit Separator

/*
**
** PAQUET : SOH size (cmd + 128) [(ocp + 128)] [STX] [...] [US] [...] [ETX] ENQ
**
*/


/*
**** History
*/

# define NOFLAG 0x00
# define ADD	0x01
# define NEXT	0x02
# define PREV	0x04
# define FLUSH	0x08
# define RESET	0x10



typedef	struct	s_hist
{
	t_list		*prev;
	char		*cmd;
}				t_hist;

char	*history(t_vector *line, uint8_t flag);

/*
**** COMPLETION
*/

# define REINIT	0x00
# define GET	0x01
# define SET	0x02

int8_t	completion(t_vector *vct);
int8_t	print_completion(t_list *list);
size_t	get_max_len(size_t len, uint8_t flag);

/*
**** BUILT_IN
*/

t_cmd		*get_cmd_struct(enum e_cmd_type type, t_vector *arg);
t_vector	*generate_bytecode(t_cmd *cmd, int ocp);

t_vector	*blt_add(t_cmd *cmd);
t_vector	*blt_avail(t_cmd *cmd);
t_vector	*blt_clear(t_cmd *cmd);
t_vector	*blt_exit(t_cmd *cmd);
t_vector	*blt_fg(t_cmd *cmd);
t_vector	*blt_help(t_cmd *cmd);
t_vector	*blt_maintail(t_cmd *cmd);
t_vector	*blt_open(t_cmd *cmd);
t_vector	*blt_pid(t_cmd *cmd);
t_vector	*blt_quit(t_cmd *cmd);
t_vector	*blt_reload(t_cmd *cmd);
t_vector	*blt_remove(t_cmd *cmd);
t_vector	*blt_reread(t_cmd *cmd);
t_vector	*blt_restart(t_cmd *cmd);
t_vector	*blt_shutdown(t_cmd *cmd);
t_vector	*blt_signal(t_cmd *cmd);
t_vector	*blt_start(t_cmd *cmd);
t_vector	*blt_status(t_cmd *cmd);
t_vector	*blt_stop(t_cmd *cmd);
t_vector	*blt_tail(t_cmd *cmd);
t_vector	*blt_update(t_cmd *cmd);
t_vector	*blt_version(t_cmd *cmd);

/*
**** Help
*/

void		help_add(void);
void		help_avail(void);
void		help_clear(void);
void		help_exit(void);
void		help_fg(void);
void		help_help(void);
void		help_maintail(void);
void		help_open(void);
void		help_pid(void);
void		help_quit(void);
void		help_reload(void);
void		help_remove(void);
void		help_reread(void);
void		help_restart(void);
void		help_shutdown(void);
void		help_signal(void);
void		help_start(void);
void		help_status(void);
void		help_stop(void);
void		help_tail(void);
void		help_update(void);
void		help_version(void);

void		del_completion_list(void *mem, size_t content_size);

# endif
