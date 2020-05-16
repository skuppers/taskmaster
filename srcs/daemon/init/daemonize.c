/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemonize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 14:47:31 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void	set_uid(const char *uid)
{
	if (uid != NULL && setuid(ft_atoi(uid)) != SUCCESS)
	{
		tlog(E_LOGLVL_ERRO, "taskmasterd: cannot change user to UID %s\n", uid);
		exit_routine(E_LOGLVL_ERRO, strerror(errno));
	}
}

static void	do_chdir(const char *directory)
{
	if (directory != NULL && chdir(directory) != SUCCESS)
	{
		tlog(E_LOGLVL_ERRO, "taskmasterd: cannot chdir to %s\n", directory);
		exit_routine(E_LOGLVL_ERRO, strerror(errno));
	}
	else
		tlog(E_LOGLVL_INFO, "changed working directory to %s\n", directory);
	
}

void		daemonize(void)
{
	set_uid(g_denv->opt.str[USER]);
	do_chdir(g_denv->opt.str[DIRECTORY]);
	umask(g_denv->opt.umask);
	g_denv->dfl_socket = DFL_SOCKET;
	if ((g_denv->opt.optmask & OPT_NODAEMON) == false)
		daemon(1, 0);
	make_socket();
	bind_socket();
	launch_jobs();
	listen_for_data();
}
