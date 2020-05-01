/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:36:21 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/01 15:18:29 by ffoissey         ###   ########.fr       */
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
# include "dictionary.h"
# include "iniparser.h"
# include "common.h"

/******************* STRUCTURES *****************/

typedef	struct			s_options
{
	mode_t				umask;
	uint8_t				loglevel;
	uint8_t				nodeamon;
	uint16_t			padding;
	char				*logfile;
	char				*configfile;
	char				*user;
	char				*directory;
	char				*childlogdir;
	char				**environ;
}						t_options;

typedef struct			s_group
{
	char				*groupname;
}						t_group;

typedef struct			s_program
{
	char				*jobname;
}						t_program;


typedef struct     		s_env
{
    int32_t				unix_socket;
	int32_t				log_fd;
	
	t_options			opt;
	struct sockaddr_un	addr;

	uint16_t	padding;

	dictionary			*dict;

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
# define DFL_SOCKET 		"/tmp/taskmaster.d/taskmaster.sock"

void						set_taskmasterd_defautls(t_env *env);

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


uint8_t					get_nodaemon(char *str);
uint32_t				get_umask(char *str);
char					**get_environ(char *str);

void					parse_ini_file(t_env *env, dictionary *dict);
dictionary 				*load_ini_file(char *str);
void 					free_inifile(dictionary *dict);

/**************************************************/


/********************** LOGGER ********************/

# define TIMEBUFFERSZ			64

# define LOG_DEBG		"[DEBUG]    "
# define LOG_INFO		"[INFO]     "
# define LOG_WARN		"[WARNING]  "
# define LOG_ERRO		"[ERROR]    "
# define LOG_CRIT		"[CRITICAL] "

enum	e_loglevel
{
	CRIT,
	ERRO,
	WARN,
	INFO,
	DEBG
};

int8_t					init_log(t_env *env);
void					print_log(t_env *env, const char *priority,
							const char *message, ...);
uint8_t					get_loglevel(char *str);
void					taskmaster_fatal(char *failed_here, char *message);

/*****************************************************/


t_cmd					*decode_cmd(t_vector *trame);
void					exit_routine(void);

# endif
