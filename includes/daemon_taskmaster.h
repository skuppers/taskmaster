/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:36:21 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 14:18:54 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DAEMON_TASKMASTER_H
# define DAEMON_TASKMASTER_H

# include <unistd.h>
# include <sys/socket.h>
# include <sys/un.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <time.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <signal.h>
# include "dictionary.h"
# include "iniparser.h"
# include "common.h"
# include <assert.h>

# define DELIMITER_STR	","
# define DELIMITER_CHAR	','

/******************* STRUCTURES *****************/

typedef	struct			s_options
{
	uint64_t			optmask;
	mode_t				umask;
	uint32_t			padding;
	char				*str[8];
	char				*environ;  //TODO
}						t_options;

typedef struct			s_group
{
	t_list				*prog_list;
	uint16_t			priority;

	char	pad[6];
	
	char				*name;
	char				*programs;
}						t_group;

typedef struct			s_instance
{
	time_t				start_time;
	time_t				stop_time;
	time_t				uptime;

	int32_t				exitcode;
	pid_t				pid;
	uint16_t			id;
	uint8_t				state;
	uint8_t				backoff;
	char pad[4];
	char				*name;
	
	struct s_instance	*next;
}						t_instance;

typedef struct			s_program
{
	mode_t					umask;
	uint16_t				priority;
	uint8_t					startsecs;			// Done
	uint8_t					stopwaitsecs;		// TODO: SIGCHLD

	char					*name;					// [program.name]
	char					*command;				// command=/usr/bin/cat syslog
	char					*bin;					// /usr/bin/cat
	char					**avs;					// [1] syslog
	
	char					*user;				
	char					*directory;
	char					*stdout_logfile;		// should be done
	char					*stderr_logfile;		// should be done

	uint8_t					numprocs;			// Done
	uint8_t					stopsignal;			// Done
	uint8_t					autostart;			// Done
	uint8_t					startretries; // START-retries (not reached running state)
	
	uint8_t					redirect_stderr;
	uint8_t					autorestart;
	char					padding[2];

	char					*environ;	
	t_list					*env;
	t_instance				*instance;				// Simple list
	char					**exitcodes;
}							t_program;

typedef struct     			s_env
{
	int						sig_tmp;
	volatile sig_atomic_t	sigint;
    int32_t					unix_socket;
	int32_t					log_fd;
	
	t_list					*environ;
	t_list					*prgm_list;
	t_list					*group_list;
	dictionary				*dict;

	t_options				opt;
	struct sockaddr_un		addr;
	uint8_t					client_connected;
	char					padding[1];
}                  			t_env;

/*************************************************/


/******************** GLOBALS ********************/

extern	t_env			*g_env;

/*************************************************/


/*********************  JOBS  ********************/

# define STATE_STARTING	"STARTING"
# define STATE_BACKOFF	"BACKOFF"
# define STATE_RUNNING	"RUNNING"
# define STATE_STOPPING	"STOPPING"
# define STATE_STOPPED	"STOPPED"
# define STATE_EXITED	"EXITED"
# define STATE_FATAL	"FATAL"
# define STATE_UNKNOWN	"UNKNOWN"

# define ERR_UNDEF_INST	3
# define ERR_FORK		2
# define ERR_EXEC		1
# define ERR_STARTING	-1
# define ERR_BACKOFF	-2
# define ERR_RUNNING	-3
# define ERR_STOPPING	-4
# define ERR_STOPPED	-5
# define ERR_FATAL		-6
# define ERR_EXITED		-7

# define UNEXPECTED		2

enum e_prg_state
{
	E_STARTING,
	E_BACKOFF,
	E_RUNNING,
	E_STOPPING,
	E_STOPPED,
	E_EXITED,
	E_FATAL,
	E_UNKNOWN
};

t_instance				*new_instance(uint8_t id, char *prog_name);
int8_t					del_instance(t_program *prg, uint8_t id);
int8_t					add_instance(t_program *prg, t_instance *inst);
int8_t					start_instance(t_program *prog, uint8_t id, t_list *environ);
int8_t					stop_instance(t_program *prog, t_instance *instance, int signo);

t_instance				*get_instance(t_program *prg, uint8_t id);
char					*get_instance_state(t_instance *instance);
void    				launch_jobs(t_env *env);
int     				child_process(t_program *prog, t_instance *instance, t_list *env);
int8_t					waiter(t_env *env);
void					update_instance_uptime(t_instance *instance);


int8_t		is_expected_exitcode(t_program *prg, t_instance *inst);

void	print_cmd_success(char *cmd, int ls, t_program *pg, uint8_t	nb);

/*************************************************/


/********************* DEFAULTS ******************/

# define DFL_WORKDIR		"/tmp/taskmaster.d/"
# define DFL_CHLDLOGDIR		"/tmp/taskmaster.d/childlog/"
# define DFL_CONFIGURATION	"/tmp/taskmaster.d/taskmasterd.conf"
# define DFL_LOGFILE		"/tmp/taskmaster.d/taskmaster.log"
# define DFL_SOCKET 		"/tmp/taskmaster.d/taskmasterd.sock"
# define DFL_LOGLVL			"info"
# define DFL_UMASK			022

void						set_taskmasterd_defautls(t_env *env);
void						check_dflt_directory(void);

int							daemonize(t_env *env);

/*************************************************/


/******************* NETWORKING ******************/

# define MAX_CLIENTS			1
# define SEND_RETRYS			3
# define SEND_PARTIAL_RETRYS	5

int8_t					make_socket(t_env *env, char *socketpath);
int8_t					bind_socket(t_env *env);
void 					listen_for_data(t_env *env);

/**************************************************/


/******************* OPTIONS & INI ****************/

# define NO_OPT					0x000
# define OPT_HELP				0x001
# define OPT_NODAEMON			0x002
# define OPT_VERSION			0x004
# define OPT_NOCLEAN			0x008

# define OPT_CONFIG				0x010 // Omit
# define OPT_USER				0x020
# define OPT_DIRECTORY			0x040
# define OPT_LOGFILE			0x080
# define OPT_LOGLVL				0x100
# define OPT_CHLDLOGDIR			0x200
# define OPT_MINFDS				0x400	//TODO
# define OPT_MINPROCS			0x800	//TODO

# define CONFIGURATION	0
# define USER			1
# define DIRECTORY		2
# define LOGFILE		3
# define LOGLEVEL		4
# define CHILDLOGDIR	5
# define MINFDS			6	// TODO
# define MINPROCS		7	// TODO

void					print_help(void);
void					print_version(void);
void					get_opt(t_env *env, int ac, char **av);
int8_t					check_opt(t_env *env);

uint8_t					get_nodaemon(char *str);
uint32_t				get_umask(char *str);
char					**get_environ(char *str);
void					taskmasterd_override(t_env *env, dictionary *dict);
void					parse_ini_file(t_env *env, dictionary *dict);
dictionary 				*load_ini_file(char *str);
void 					free_inifile(dictionary *dict);

/**************************************************/


/********************** LOGGER ********************/

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

enum	e_loglvl
{
	E_LOGLVL_DEBG,
	E_LOGLVL_INFO,
	E_LOGLVL_WARN,
	E_LOGLVL_ERRO,
	E_LOGLVL_CRIT
};


int8_t					init_log(t_env *env);
void					tlog(t_env *env, uint8_t loglvl,
							const char *message, ...);

/*****************************************************/

void					init_signals(void);
void					exit_routine(void);


/*
************************ ENV
*/

t_list		*envtolst(char **tab);
char		**envtotab(t_list *lst);
void		print_lst(t_list *lst);
char		*get_var(t_list *intern, char *name);
int8_t		add_var(t_list **alst, char *name, char *data);
int8_t		add_var_vct(t_list **alst, t_vector *val);
void		strvalue_to_lst(t_list **lst, char *str);

void		free_node(void *node, size_t content_size);
int8_t		free_var(t_list **alst, char *name);
void		free_env(void *node, size_t content_size);
void		del_group(void *node, size_t content_size);
void		del_prgm(void *node, size_t content_size);

typedef struct					s_variable
{
	char						*name;
	char						*data;
}								t_variable;

/*
************************ INIPARSER
*/

dictionary 	*load_ini_file(char *str);
int32_t		get_secint(dictionary *dict, char *secname, char *key);
int8_t		get_secbool(dictionary *dict, char *secname, char *key);
char		*get_secstring(dictionary *dict, char *secname, char *key);

void		set_grp_list(t_env *env);

/*
************************ BUILTIN
*/

# define ERR_MSG	0
# define INFO_MSG	1

typedef	t_vector	*(*t_process_cmd)(t_cmd *);

t_vector				*execute_cmd(t_cmd *cmd);

t_vector			*cmd_add(t_cmd *cmd);
t_vector			*cmd_avail(t_cmd *cmd);
t_vector			*cmd_clear(t_cmd *cmd);
t_vector			*cmd_fg(t_cmd *cmd);
t_vector			*cmd_maintail(t_cmd *cmd);
t_vector			*cmd_pid(t_cmd *cmd);
t_vector			*cmd_reload(t_cmd *cmd);
t_vector			*cmd_remove(t_cmd *cmd);
t_vector			*cmd_reread(t_cmd *cmd);
t_vector			*cmd_restart(t_cmd *cmd);
t_vector			*cmd_shutdown(t_cmd *cmd);
t_vector			*cmd_signal(t_cmd *cmd);
t_vector			*cmd_start(t_cmd *cmd);
t_vector			*cmd_status(t_cmd *cmd);
t_vector			*cmd_stop(t_cmd *cmd);
t_vector			*cmd_tail(t_cmd *cmd);
t_vector			*cmd_update(t_cmd *cmd);
t_vector			*cmd_version(t_cmd *cmd);

typedef	t_vector	*(*t_action)(t_instance *, t_program *program);

t_vector	*exec_action_args(char **arg, int ac, t_action to_do);
t_vector	*exec_action_all(t_action to_do);
t_vector	*get_msg(char *name, char *msg, uint8_t flag);

# endif
