/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_routine_d.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 12:59:06 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void	free_arg_vct(void)
{
	int		i;

	i = 0;
	while (i < g_denv->ac)
		vct_del(g_denv->av + i++);
	free(g_denv->av);
}

static int	reload_daemon(void)
{
	int		fd_log;
	int		i;
	char	**av;

	free_inifile(g_denv->dict);
	fd_log = g_denv->log_fd;
	av = (char **)malloc(sizeof(char *) * (g_denv->ac + 1));
	if (av != NULL)
	{
		av[g_denv->ac] = NULL;
		i = 0;
		while (i < g_denv->ac)
		{
			av[i] = vct_dupstr(g_denv->av[i]);
			i++;
		}
		bzero(g_denv, sizeof(t_denv));
	}
	free_arg_vct();
	if (av != NULL)
		execve(av[0], av, g_denv->environ_tab);
	ft_free_tab_str(av);
	tlog(E_LOGLVL_CRIT, "Failed to reload: %s: %s\n", av[0], strerror(errno));
	close(fd_log);
	exit(EXIT_FAILURE);
}

void		general_stuff_exit(void)
{
	close(g_denv->unix_socket);
	ft_lstdel(&g_denv->environ, free_env);
	ft_lstdel(&g_denv->prgm_list, del_prgm);
	if (g_denv->dfl_socket != NULL)
		unlink(g_denv->dfl_socket);
	if (g_tmpenv != NULL)
	{
		ft_lstdel(&g_tmpenv->prgm_list, del_prgm);
		free_inifile(g_tmpenv->dict);
		free(g_tmpenv);
	}
	flock(g_denv->lock, LOCK_UN);
}

void		exit_routine(const int flag, ...)
{
	va_list	arg;
	char	*err_str;
	int		ret;

	general_stuff_exit();
	ret = EXIT_SUCCESS;
	if (flag != NO_MSG)
	{
		va_start(arg, flag);
		err_str = va_arg(arg, char *);
		if (err_str != NULL)
			tlog(flag, "%s\n", err_str);
		va_end(arg);
		ret = EXIT_FAILURE;
	}
	else
	{
		if (g_denv->shutdown == S_RELOAD)
			reload_daemon();
	}
	free_inifile(g_denv->dict);
	free_arg_vct();
	close(g_denv->log_fd);
	exit(ret);
}
