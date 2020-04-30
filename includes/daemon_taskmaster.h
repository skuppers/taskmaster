/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:36:21 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 20:22:56 by ffoissey         ###   ########.fr       */
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

# include "libft.h"
# include "stdint.h"

# define DFLT_SOCKET	"/tmp/taskmstsock"
# define DFLT_LOGPATH	"/tmp/tasklog"

# define LOG_INFO		"[INFO]    "
# define LOG_WARN		"[WARNING] "
# define LOG_ERR		"[ERROR]   "

typedef struct     		s_env
{
    int32_t				unix_socket;
	int32_t				log_fd;
	struct sockaddr_un	addr;
}                  		t_env;

int8_t					init_log(t_env *env);

# endif