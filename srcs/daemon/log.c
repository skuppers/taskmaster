/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 21:02:35 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

uint8_t	get_loglevel(char *str)
{
	if (ft_strequ(str, LOGLVL_DEBG) == 1)
		return (E_LOGLVL_DEBG);
	if (ft_strequ(str, LOGLVL_INFO) == 1)
		return (E_LOGLVL_INFO);
	if (ft_strequ(str, LOGLVL_WARN) == 1)
		return (E_LOGLVL_WARN);
	if (ft_strequ(str, LOGLVL_ERRO) == 1)
		return (E_LOGLVL_ERRO);
	if (ft_strequ(str, LOGLVL_CRIT) == 1)
		return (E_LOGLVL_CRIT);
	return (E_LOGLVL_DEBG);
}

char	*loglvl_tostr(uint8_t loglvl)
{
	if (loglvl == E_LOGLVL_DEBG)
		return (LOG_DEBG_STR);
	if (loglvl == E_LOGLVL_INFO)
		return (LOG_INFO_STR);
	if (loglvl == E_LOGLVL_WARN)
		return (LOG_WARN_STR);
	if (loglvl == E_LOGLVL_ERRO)
		return (LOG_ERRO_STR);
	if (loglvl == E_LOGLVL_CRIT)
		return (LOG_CRIT_STR);
	return (LOG_DEBG_STR);
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

int			tlog(int loglvl, const char *message, ...)
{
	
	va_list			args;
	char			time_buffer[TIMEBUFFERSZ];

	if (loglvl >= get_loglevel(g_denv->opt.str[LOGLEVEL]))
	{
		ft_bzero(&time_buffer, TIMEBUFFERSZ);
		getstr_time(time_buffer);
		ft_dprintf(g_denv->log_fd, "[%s] %s- ", time_buffer, loglvl_tostr(loglvl));
		va_start(args, message);
		vdprintf(g_denv->log_fd, message, args);
		va_end(args);
	}
	return (SUCCESS);
}

int8_t      init_log()
{
    int32_t     debug_fd;

	if (g_denv->opt.optmask & OPT_NODAEMON)
		debug_fd = STDERR_FILENO;
	else
    	debug_fd = open(g_denv->opt.str[LOGFILE], O_RDWR | O_APPEND | O_CREAT, 0644);
	if (debug_fd < 0)
	{
		dprintf(STDERR_FILENO, "Error: Could not create log file: %s\n", strerror(errno));
		return (-1);
	}
	g_denv->log_fd = debug_fd;
	tlog(E_LOGLVL_INFO, "[ Taskmaster startup ]\n");
	tlog(E_LOGLVL_INFO, "Taskmaster logger is up\n");
	return (0);
}
