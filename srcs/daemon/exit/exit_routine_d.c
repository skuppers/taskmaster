/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_routine_d.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/16 10:28:43 by ffoissey         ###   ########.fr       */
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

static void	fail_execve(char **av, char **env)
{
	if (av == NULL || av[0] == NULL)
		tlog(E_LOGLVL_CRIT, "Failed to reload taskmasterd\n");
	else
		tlog(E_LOGLVL_CRIT, "Failed to reload `%s': %s\n", av[0],
							strerror(errno));
	ft_free_tab_str(env);
	ft_free_tab_str(av);
	ft_lstdel(&g_denv->environ, free_env);
	close(g_denv->log_fd);
	exit(EXIT_FAILURE);
}

static void	reload_daemon(void)
{
	int		i;
	char	**av;
	char	**env;

	free_inifile(g_denv->dict);
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
	}
	env = envtotab(g_denv->environ);
	free_arg_vct();
	if (av != NULL && env != NULL)
		execve(av[0], av, env);
	fail_execve(av, env);
}

void		general_stuff_exit(void)
{
	close(g_denv->unix_socket);
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
	ft_lstdel(&g_denv->environ, free_env);
	free_inifile(g_denv->dict);
	free_arg_vct();
	close(g_denv->log_fd);
	exit(ret);
}
