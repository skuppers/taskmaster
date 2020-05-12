/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_routine_d.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 17:45:20 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void	free_arg_vct(void)
{
	int		i;

	i = 0;
	while (i < g_denv->ac)
	{
		vct_del(&g_denv->av[i]);
		i++;
	}
	free(g_denv->av);
}

static int	reload_daemon(void)
{
	int		fd_log;
	int		i;
	char	**av;

	fd_log = g_denv->log_fd;
	av = (char **)malloc(sizeof(char *) * (g_denv->ac + 1));
	av[g_denv->ac] = NULL; // PROTECT MALLOC
	i = 0;
	while (i < g_denv->ac)
	{
		av[i] = vct_dupstr(g_denv->av[i]);
		i++;
	}
	free_arg_vct();
	bzero(g_denv, sizeof(t_denv));
	bzero(g_newenv, sizeof(t_denv));
	execve(av[0], av, g_denv->environ_tab);
	ft_free_tab_str(av);
	tlog(E_LOGLVL_CRIT, "Failed to reload: %s: %s\n", av[0], strerror(errno));
	close(fd_log);
	exit(EXIT_FAILURE);
}

void		exit_routine(const int flag, ...)
{
	va_list	arg;
	char	*err_str;

    if (g_denv->dict != NULL)
	    free_inifile(g_denv->dict);
	close(g_denv->unix_socket);
	unlink(DFL_SOCKET);
	ft_lstdel(&g_denv->environ, free_env);
	ft_lstdel(&g_denv->prgm_list, del_prgm);
	ft_lstdel(&g_newenv->prgm_list, del_prgm);
	flock(g_denv->lock, LOCK_UN);
	// free vector tab arg
	if (flag != NO_MSG)
	{
		va_start(arg, flag);
		err_str = va_arg(arg, char *);
		if (err_str != NULL)
			tlog(flag, "%s\n", err_str);
		va_end(arg);
		close(g_denv->log_fd);
		free_arg_vct();
		exit(EXIT_FAILURE);
	}
	if (g_denv->shutdown == S_RELOAD)
		reload_daemon();
	free_arg_vct();
	close(g_denv->log_fd);
	exit(EXIT_SUCCESS);
}
