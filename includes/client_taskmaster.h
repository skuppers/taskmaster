/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_taskmaster.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:36:21 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/16 12:55:40 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_TASKMASTER_H
# define CLIENT_TASKMASTER_H

# include <termios.h>
# include <unistd.h>
# include <sys/un.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <sys/ioctl.h>
# include "dictionary.h"
# include "iniparser.h"
# include "common.h"

/*
********* ENV
*/

# define NB_STR_CONF		3
# define AK_AMOUNT			14

# define DEL_FEEDBACK		0
# define KEEP_FEEDBACK		1

# define RESTART_TIME		3

# define TAIL_FIFO			0x01
# define TAIL_NB			0x02

# define BEFORE				0
# define AFTER				1

typedef struct s_env		t_env;

typedef int8_t				(*t_actionkeys)(t_env *env, t_vector *vct,
								char c[BUFF_SIZE]);

typedef	struct				s_opt
{
	char					*str[NB_STR_CONF];
	t_vector				*batch_cmd;
	uint64_t				mask;
}							t_opt;

struct						s_env
{
	struct termios			*orig;
	struct termios			*taskmst;
	t_cmd					*cmd;
	t_opt					opt;
	dictionary				*dict;
	t_vector				*cur_line;
	uint64_t				ak_masks[AK_AMOUNT];
	t_actionkeys			actionkeys[AK_AMOUNT];
	uint32_t				cursoridx;
	uint16_t				cursorx;
	uint16_t				cursory;
	unsigned short int		winwid;
	unsigned short int		winhei;

	volatile sig_atomic_t	sigint;
	volatile sig_atomic_t	sigpipe;
	volatile sig_atomic_t	sigwinch;
	uint8_t					szchanged;

	char					pad[3];
	int32_t					unix_socket;
	uint64_t				flag_exec;
};

extern t_env				*g_env;

/*
*************************** INIT
*/

/*
*** init.c
*/

void						init(int ac, char **av);

/*
*** termmode.c
*/

# define NEW				0x01
# define RELEASE			0x02

void						create_termmode(void);
void						apply_termmode(const uint8_t flag);
void						canonic_mode(const bool flag);

/*
*** opt.c
*/

# define NB_OPT				10
# define NB_NOCONF_OPT		6

# define NO_OPT				0x0000
# define OPT_HELP			0x0001
# define OPT_INTERACTIVE	0x0002
# define OPT_DEBUG			0x0004
# define OPT_SERVERURL		0x0008
# define OPT_CONFIGURATION	0x0010
# define OPT_BATCHCMD		0x0020
# define OPT_PROMPT			0x0040

# define SERVERURL			0
# define CONFIGURATION		1
# define PROMPT				2

# define UNIX_URI			"unix://"
# define UNIX_URI_SIZE		7

# define DFL_PROMPT			"taskmaster"
# define DFL_URL			"/tmp/taskmaster.d/taskmasterd.sock"
# define DFL_CONFIGURATION	"/tmp/taskmaster.d/taskmasterd.conf"

# define SERVERURL_SEC		"taskmasterctl:serverurl"
# define PROMPT_SEC			"taskmasterctl:prompt"

void						get_opt(const int ac, char **av);

/*
*** print_help.c
*/

void						print_help(void);

/*
*** load_config.c
*/

void						load_config(void);

/*
*** client_signal.c
*/

void						init_signals(void);
void						sigwinch_handle(int signo);

/*
*************************** LINE EDITION
*/

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

enum						e_action_keys
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
	AK_TABULATION
};

int							tsk_readline(t_vector *vct, const int fd,
								t_env *env);
void						assign_keycodes(void);
void						link_keys_functions(
								t_actionkeys actionkeys[AK_AMOUNT]);
void						update_winsize(t_env *env);
void						dec_x(t_env *env, int goup);
void						inc_x(t_env *env, int godown);
void						calc_after_totalprint(t_env *env, t_vector *vct);
void						handle_actionkey(t_env *env, char c[BUFF_SIZE],
								t_vector *vct);
uint64_t					compute_mask(char c[BUFF_SIZE]);
int							print_prompt(t_env *env);
int							tsk_readline(t_vector *vct,
								const int fd, t_env *env);
void						pinc_x(t_env *env);
int8_t						handle_signal(t_env *env, t_vector *vct);
int8_t						signal_catched(t_env *env);
int8_t						putchar_in_vct(t_env *env, t_vector *dest,
								char *src, size_t size);

/*
*** Action Key
*/

int8_t						ak_arrow_up(t_env *env,
								t_vector *vct, char c[BUFF_SIZE]);
int8_t						ak_arrow_down(t_env *env,
								t_vector *vct, char c[BUFF_SIZE]);
int8_t						ak_arrow_left(t_env *env,
								t_vector *vct, char c[BUFF_SIZE]);
int8_t						ak_arrow_right(t_env *env,
								t_vector *vct, char c[BUFF_SIZE]);
int8_t						ak_home(t_env *env,
								t_vector *vct, char c[BUFF_SIZE]);
int8_t						ak_end(t_env *env,
								t_vector *vct, char c[BUFF_SIZE]);
int8_t						ak_delete(t_env *env,
								t_vector *vct, char c[BUFF_SIZE]);
int8_t						ak_backspace(t_env *env,
								t_vector *vct, char c[BUFF_SIZE]);
int8_t						ak_ctrl_d(t_env *env,
								t_vector *vct, char c[BUFF_SIZE]);
int8_t						ak_ctrl_l(t_env *env,
								t_vector *vct, char c[BUFF_SIZE]);
int8_t						ak_ctrl_r(t_env *env,
								t_vector *vct, char c[BUFF_SIZE]);
int8_t						ak_hightab(t_env *env,
								t_vector *vct, char c[BUFF_SIZE]);

/*
*************************** CONNECT
*/

void						connect_to_daemon(const char *socketpath);
void						restart_connection(void);
t_vector					*get_feedback(void);

/*
*************************** ROUTINE
*/

/*
*** exec_routine.c
*/

t_vector					*routine(t_vector *line, const uint8_t flag);
void						read_cmd(void);
void						exec_tail(uint64_t flag, t_vector *feedback);

/*
*** get_cmd.c
*/

t_cmd						*get_cmd(t_vector *line);
t_vector					*get_status(const uint8_t flag);

/*
*** exit_routine.c
*/

void						reset_cmd(void);
void						exit_routine(const int flag, ...);

/*
*************************** HISTORY
*/

# define NOFLAG 			0x00
# define ADD_ENTRY			0x01
# define NEXT				0x02
# define PREV				0x04
# define FLUSH				0x08
# define RESET				0x10

typedef	struct				s_hist
{
	t_list					*prev;
	char					*cmd;
}							t_hist;

char						*history(t_vector *line, const uint8_t flag);

/*
*************************** COMPLETION
*/

# define REINIT				0x00
# define GET				0x01
# define SET				0x02

enum	e_comp_type
{
	NO_COMP,
	CMD_COMP,
	INSTANCE_COMP,
	PROG_COMP
};

int8_t						completion(t_vector *vct);
bool						is_node_uniq(t_list *lst, char *name);
int8_t						print_completion(t_list *list);
size_t						get_max_len(const size_t len, const uint8_t flag);
void						del_completion_list(void *mem, size_t content_size);
char						*get_last_word(t_vector *vct);
enum e_comp_type			get_type_of_completion(t_vector	*vct);
t_list						*prog_comp(char *last_word);
t_list						*instance_comp(char *last_word);
void						lst_add_node(t_list **list, const char *str);

typedef t_list				*(*t_comp_type)(char *);
typedef void				(*t_fill_list)(t_list **, char *, t_vector *);

/*
*************************** BUILTIN
*/

typedef	t_vector			*(*t_builtin)(t_cmd *);
typedef	void				(*t_help)(void);

t_vector					*blt_add(t_cmd *cmd);
t_vector					*blt_avail(t_cmd *cmd);
t_vector					*blt_clear(t_cmd *cmd);
t_vector					*blt_exit(t_cmd *cmd);
t_vector					*blt_fg(t_cmd *cmd);
t_vector					*blt_help(t_cmd *cmd);
t_vector					*blt_maintail(t_cmd *cmd);
t_vector					*blt_open(t_cmd *cmd);
t_vector					*blt_pid(t_cmd *cmd);
t_vector					*blt_quit(t_cmd *cmd);
t_vector					*blt_reload(t_cmd *cmd);
t_vector					*blt_remove(t_cmd *cmd);
t_vector					*blt_reread(t_cmd *cmd);
t_vector					*blt_restart(t_cmd *cmd);
t_vector					*blt_shutdown(t_cmd *cmd);
t_vector					*blt_signal(t_cmd *cmd);
t_vector					*blt_start(t_cmd *cmd);
t_vector					*blt_status(t_cmd *cmd);
t_vector					*blt_stop(t_cmd *cmd);
t_vector					*blt_tail(t_cmd *cmd);
t_vector					*blt_update(t_cmd *cmd);
t_vector					*blt_version(t_cmd *cmd);

/*
**** Help
*/

void						help_add(void);
void						help_avail(void);
void						help_clear(void);
void						help_exit(void);
void						help_fg(void);
void						help_help(void);
void						help_maintail(void);
void						help_open(void);
void						help_pid(void);
void						help_quit(void);
void						help_reload(void);
void						help_remove(void);
void						help_reread(void);
void						help_restart(void);
void						help_shutdown(void);
void						help_signal(void);
void						help_start(void);
void						help_status(void);
void						help_stop(void);
void						help_tail(void);
void						help_update(void);
void						help_version(void);

#endif
