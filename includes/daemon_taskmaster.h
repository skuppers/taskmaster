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

# define LOG_INFO		"[INFO]    "
# define LOG_WARN		"[WARNING] "
# define LOG_ERR		"[ERROR]   "

# define TIMEBUFFERSZ			64

# define MAX_CLIENTS			1
# define SEND_RETRYS			3
# define SEND_PARTIAL_RETRYS	5

# define DFL_WORKDIR	"/tmp/taskmaster.d/"

# define DFL_CONFIGURATION	"/tmp/taskmaster.d/taskmasterd.conf"
# define TMP_DFL_CONF	"/tmp/taskmaster.d/taskmaster.conf"
# define TMP_DFL_SOCKET "/tmp/taskmaster.d/taskmaster.sock"
# define TMP_DFL_LOG	"/tmp/taskmaster.d/taskmaster.log"

typedef	struct			s_options
{
	uint16_t			logfile_maxbytes;
	uint16_t			umask;
	uint8_t				logfile_backups;
	uint8_t				loglevel; // Enum ici
	uint8_t				nodeamon;
	uint8_t		padding;
	char				*logfile;
	char				*pidfile;
	char				*user;
	char				*directory;
	char				*childlogdir;
	char				*environ;
}						t_options;

typedef struct     		s_env
{
    int32_t				unix_socket;
	int32_t				log_fd;
	
	t_options			opt;
	struct sockaddr_un	addr;

	uint16_t	padding;

	dictionary			*dict;

}                  		t_env;

extern	t_env			*g_env;

int8_t					init_log(t_env *env);
void					print_log(t_env *env, const char *priority, const char *message, ...);
void					taskmaster_fatal(char *failed_here, char *message);

int8_t					make_socket(t_env *env, char *socketpath);
int8_t					bind_socket(t_env *env);

void 					listen_for_data(t_env *env);

t_cmd					*decode_cmd(t_vector *trame);

void					parse_ini_file(t_env *env, dictionary *dict);
dictionary 				*load_ini_file(char *str);
void 					free_inifile(dictionary *dict);

void					exit_routine(void);

# endif
