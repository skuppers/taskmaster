/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/15 17:12:50 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void		getstr_time(char *buffer)
{
	struct timeval	tv;
	time_t			nowtime;
	struct tm		*nowtm;
	char			tmbuf[64];

	gettimeofday(&tv, NULL);
	nowtime = tv.tv_sec;
	nowtm = localtime(&nowtime);
	strftime(tmbuf, sizeof(tmbuf), "%Y-%m-%d %H:%M:%S", nowtm);
	snprintf(buffer, TIMEBUFFERSZ, "%s.%06ld", tmbuf, tv.tv_usec);
}

int				tlog(int loglvl, const char *message, ...)
{
	va_list			args;
	char			time_buffer[TIMEBUFFERSZ];

	if (loglvl <= get_loglevel(g_denv->opt.str[LOGLEVEL]))
	{
		bzero(&time_buffer, TIMEBUFFERSZ);
		getstr_time(time_buffer);
		dprintf(g_denv->log_fd, "[%s] %s- ", time_buffer,
							loglvl_todefine(loglvl));
		va_start(args, message);
		vdprintf(g_denv->log_fd, message, args);
		va_end(args);
	}
	return (SUCCESS);
}

static int		unlink_rec(const char *fpath, const struct stat *sb,
						int typeflag, struct FTW *ftwbuf)
{
	(void)sb;
	(void)typeflag;
	(void)ftwbuf;
	return (remove(fpath));
}

void			init_log(void)
{
	nftw(g_denv->opt.str[CHILDLOGDIR], unlink_rec, 32, FTW_DEPTH | FTW_PHYS);
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
