/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:36:21 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/02 19:24:17 by ffoissey         ###   ########.fr       */
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
# include <time.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <signal.h>
# include "dictionary.h"
# include "iniparser.h"
# include "common.h"

/******************* STRUCTURES *****************/

typedef	struct			s_options
{
	uint64_t			optmask;
	mode_t				umask;
	uint32_t			padding;
	char				*str[8];
	char				**environ;  //TODO
}						t_options;

typedef struct			s_group
{
	t_list				*prog_list;
	uint16_t			priority;

	uint16_t	padding;
	uint32_t	pad;
	
	char				*name;
	char				*programs;
}						t_group;

typedef struct			s_program
{
	mode_t				umask;
	uint16_t			priority;
	uint16_t			startsec;
	uint16_t			startretries;
	uint8_t				numprocs;
	uint8_t				autostart;
	uint8_t				stopsignal;
	uint8_t				stopwaitsec;
	uint8_t				redirect_stderr;
	uint8_t	padding;
	char				*autorestart;
	char				*name;
	char				*command;
	char				*directory;
	char				*exitcodes;
	char				*user;
	char				*stdout_logfile;
	char				*stderr_logfile;
	char				*environ;			
}						t_program;


typedef struct     		s_env
{
    int32_t				unix_socket;
	int32_t				log_fd;
	t_options			opt;
	struct sockaddr_un	addr;
uint16_t			padding;
	t_list				*prgm_list;
	t_list				*goup_list;

	uint8_t				client_connected;
	char paddng[7];
	dictionary			*dict;
	volatile sig_atomic_t	sigint;

	uint32_t			more_padding;

}                  		t_env;

/*************************************************/


/******************** GLOBALS ********************/

extern	t_env			*g_env;


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
# define OPT_NODAEMON			0x001
# define OPT_HELP				0x002
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
void					print_log(t_env *env, uint8_t loglvl,
							const char *message, ...);
void					taskmaster_fatal(char *failed_here, char *message);

/*****************************************************/


t_cmd					*decode_cmd(t_vector *trame);
t_vector				*execute_cmd(t_cmd *cmd);

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
void		free_node(void *node);
int8_t		free_var(t_list **alst, char *name);

typedef struct					s_variable
{
	char						*name;
	char						*data;
}								t_variable;
# endif
