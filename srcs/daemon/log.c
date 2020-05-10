/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 18:50:41 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

char			*loglvl_tostr(uint8_t loglvl)
{
	static char	*str_log[] = {LOG_DEBG_STR, LOG_INFO_STR, LOG_WARN_STR,
									LOG_ERRO_STR, LOG_CRIT_STR};

	return ((loglvl < 5) ? LOG_DEBG_STR : str_log[loglvl]);
}

uint8_t			get_loglevel(char *str)
{
	uint8_t		loglvl;

	loglvl = 0;
	while (loglvl < 5)
	{
		if (ft_strequ(str, loglvl_tostr(loglvl)) == true)
			return (loglvl);
		loglvl++;
	}
	return (E_LOGLVL_DEBG);
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

int				tlog(int loglvl, const char *message, ...)
{
	
	va_list			args;
	char			time_buffer[TIMEBUFFERSZ];

	if (loglvl >= get_loglevel(g_denv->opt.str[LOGLEVEL]))
	{
		bzero(&time_buffer, TIMEBUFFERSZ);
		getstr_time(time_buffer);
		dprintf(g_denv->log_fd, "[%s] %s- ", time_buffer,
							loglvl_tostr(loglvl));
		va_start(args, message);
		vdprintf(g_denv->log_fd, message, args);
		va_end(args);
	}
	return (SUCCESS);
}

void 		     init_log()
{
	g_denv->log_fd = STDERR_FILENO;
	if ((g_denv->opt.optmask & OPT_NODAEMON) == false)
    	g_denv->log_fd = open(g_denv->opt.str[LOGFILE],
						O_RDWR | O_APPEND | O_CREAT, 0644);
	if (g_denv->log_fd == FAILURE)
	{
		dprintf(STDERR_FILENO, "Error: Could not create log file: %s\n",
				strerror(errno));
		exit_routine(ERR, NULL);
	}
	tlog(E_LOGLVL_INFO, "[ Taskmaster startup ]\n");
	tlog(E_LOGLVL_INFO, "Taskmaster logger is up\n");
	if (mkdir(g_denv->opt.str[CHILDLOGDIR], 0744) == FAILURE)
	{
		if (errno != EEXIST)
		{
			tlog(E_LOGLVL_CRIT, "Fatal error in: init_log():"
					" Could create childlog directory\n");
			exit_routine(ERR, strerror(errno));
		}
	}
}
