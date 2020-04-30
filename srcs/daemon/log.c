/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 19:05:55 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

void		taskmaster_fatal(char *failed_here, char *message)
{
	dprintf(2, "A fatal error has happened in %s: %s\n", failed_here, message);
	dprintf(2, "See logs for details\n");
}

static void		getstr_time(char *buffer)
{
	struct timeval	tv;
	time_t			nowtime;
	struct tm 		*nowtm;
	char 			tmbuf[64];

	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
	strftime(tmbuf, sizeof tmbuf, "%Y-%m-%d %H:%M:%S", nowtm);
	snprintf(buffer, TIMEBUFFERSZ, "%s.%06ld", tmbuf, tv.tv_usec);
}

void		print_log(t_env *env, const char *priority, const char *message, ...)
{
	
	va_list			args;
	char			time_buffer[TIMEBUFFERSZ];


	ft_bzero(&time_buffer, TIMEBUFFERSZ);
	getstr_time(time_buffer);
	ft_dprintf(env->log_fd, "[%s] %s- ", time_buffer, priority);
	va_start(args, message);
	vdprintf(env->log_fd, message, args);
	va_end(args);
}

int8_t      init_log(t_env *env)
{
    int32_t     debug_fd;

    debug_fd = open(DFLT_LOGPATH, O_RDWR | O_APPEND | O_CREAT, 0644);
	if (debug_fd < 0)
	{
		printf("Could not create log file.\n");
		return (-1);
	}
	env->log_fd = debug_fd;
	print_log(env, LOG_INFO, "[ Taskmaster startup ]\n");
	print_log(env, LOG_INFO, "Taskmaster logger is up\n");
	return (0);
}
