/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_daemon.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 18:27:42 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 17:44:02 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void	set_daemon_environment(char **environ)
{
	g_denv->environ = envtolst(environ);
	if (g_denv->opt.environ != NULL)
	{
		strvalue_to_lst(&g_denv->environ, g_denv->opt.environ);
	//	print_lst(g_denv->environ); // DEBUG LIST ENV
	}
}

static void	check_dflt_directory(void)
{
	DIR		*dir;
	int		lock;
	
	dir = opendir(DFL_WORKDIR);
	if (dir == NULL)
	{
		if (errno != ENOENT || mkdir(DFL_WORKDIR, 0744) == FAILURE)
		{
			dprintf(STDERR_FILENO, "taskmasterd: Fatal error in:"
				" check_working_directory(): Could not create"
				" default working directory: %s\n", strerror(errno));
			exit_routine(ERR, NULL);
		}
	}
	else
		closedir(dir);
	lock = open(DFL_LOCK, O_RDWR | O_CREAT, 0700);
	if (lock == FAILURE || flock(lock, LOCK_EX | LOCK_NB) == FAILURE)
	{
		dprintf(STDERR_FILENO, "taskmasterd: could not lock lockfile."
								" Is another daemon running?\n%s\n",
								strerror(errno));
		exit_routine(ERR, NULL);
	}
	g_denv->lock = lock;
}

static void	set_taskmasterd_defautls(void)
{
	g_denv->opt.str[CONFIGURATION] = DFL_CONFIGURATION;
    g_denv->opt.str[LOGFILE] = DFL_LOGFILE;
	g_denv->opt.str[LOGLEVEL] = DFL_LOGLVL;
	g_denv->opt.str[USER] = NULL;
	g_denv->opt.str[DIRECTORY] = NULL;
	g_denv->opt.str[CHILDLOGDIR] = DFL_CHLDLOGDIR;
    g_denv->opt.umask = DFL_UMASK;
	g_denv->opt.environ = NULL;

}

void		get_abs_pathname(t_vector *name)
{
	char		cwd[PATH_MAX];
	char		*pathname;

	if (vct_getfirstchar(name) != '/')
	{
		vct_push(name, '/');
		vct_pushstr(name, getcwd(cwd, PATH_MAX));
		pathname = canonicalize_file_name(vct_getstr(name));
		vct_clear(name);
		vct_addstr(name, pathname);
		free(pathname);
	}
}

void		init(int ac, char **av, char **environ)
{
	int i;


	bzero(g_denv, sizeof(t_denv));
	bzero(g_newenv, sizeof(t_denv));
	g_denv->environ_tab = environ;
	g_denv->av = (t_vector **)malloc(sizeof(t_vector *) * (ac + 1)); // PROTECT_MALLOC
	g_denv->av[ac] = NULL;
	i = 0;
	while (i < ac)
	{
		g_denv->av[i] = vct_newstr(av[i]);
		i++;
	}
	g_denv->av[i] = NULL;
	g_denv->ac = ac;
	get_abs_pathname(g_denv->av[0]);
	g_denv->unix_socket = -1;
	set_taskmasterd_defautls(); // DOIT ABSOLUMENT ETRE FAIT EN PREMIER
	check_dflt_directory();
	get_opt(ac - 1, av + 1);
	set_daemon_environment(environ);
	init_log();
	parse_ini_file();
	init_signals();
}
