/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaults.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 20:47:32 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void	check_dflt_directory(void)
{
	DIR* dir;
	
	dir = opendir(DFL_WORKDIR);
	if (dir)
		closedir(dir);
	else if (ENOENT == errno)
	{
		if (mkdir(DFL_WORKDIR, 0744) == -1)
		{
			dprintf(STDERR_FILENO, "Fatal error in: check_working_directory():"
				" Could not create default working directory: %s\n", strerror(errno));
			exit_routine();
		}
	}
	else
	{
    	dprintf(STDERR_FILENO, "Fatal error in: check_working_directory():"
				" Could not open default working directory: %s\n", strerror(errno));
		exit_routine();
	}
}

uint8_t	get_nodaemon(char *str)
{
	if (ft_strequ(str, "1") == TRUE || ft_strequ(str, "true") == TRUE)
		return (TRUE);
	return (FALSE);
}

uint32_t	get_umask(char *str)
{
	if (str == NULL || ft_strlen(str) > 4)
		return (22); // 22 ou 022
	if (ft_strcheck(str, ft_isdigit) == FALSE)
		return (22);
	return (ft_atoi(str));
}

char	**get_environ(char *str)
{
	(void)str;
	return (NULL);
}

void    set_taskmasterd_defautls(t_denv *env)
{
	env->opt.str[CONFIGURATION] = DFL_CONFIGURATION;
    env->opt.str[LOGFILE] = DFL_LOGFILE;
	env->opt.str[LOGLEVEL] = DFL_LOGLVL;
	env->opt.str[USER] = NULL;
	env->opt.str[DIRECTORY] = NULL;
	env->opt.str[CHILDLOGDIR] = DFL_CHLDLOGDIR;
    env->opt.umask = DFL_UMASK;
	env->opt.environ = NULL;
}

void	taskmasterd_override(t_denv *env, dictionary *dict)
{
	char	*tmp;

	if ((env->opt.optmask & OPT_NODAEMON) == FALSE)
	{
		if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:nodaemon", NULL)) != NULL)
			env->opt.optmask |= get_nodaemon(tmp);
	}

	if ((env->opt.optmask & OPT_NOCLEAN) == FALSE)
	{
		if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:nocleanup", NULL)) != NULL)
			env->opt.optmask |= get_nodaemon(tmp);
	}

	if ((env->opt.optmask & OPT_USER) == FALSE)
	{
		if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:user", NULL)) != NULL)
		{
			if (ft_strlen(tmp) <= 0)
			{
				tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd] - user cannot be blank\n");
				exit_routine();
			}
			env->opt.str[USER] = tmp;
		}
	}
	if ((env->opt.optmask & OPT_DIRECTORY) == FALSE)
	{
		if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:directory", NULL)) != NULL)
		{
			if (ft_strlen(tmp) <= 0)
			{
				tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd] - directory cannot be blank\n");
				exit_routine();
			}
			env->opt.str[DIRECTORY] = tmp;
		}
	}
	if ((env->opt.optmask & OPT_LOGFILE) == FALSE)
	{
		if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:logfile", NULL)) != NULL)
		{
			if (ft_strlen(tmp) <= 0)
			{
				tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd] - logfile cannot be blank\n");
				exit_routine();
			}
			env->opt.str[LOGFILE] = tmp;
		}
	}
	if ((env->opt.optmask & OPT_LOGLVL) == FALSE)
	{
		if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:loglevel", NULL)) != NULL)
		{
			if (ft_strlen(tmp) <= 0)
			{
				tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd] - loglevel cannot be blank\n");
				exit_routine();
			}
			env->opt.str[LOGLEVEL] = tmp;
		}
	}
	if ((env->opt.optmask & OPT_CHLDLOGDIR) == FALSE)
	{
		if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:childlogdir", NULL)) != NULL)
		{
			if (ft_strlen(tmp) <= 0)
			{
				tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd] - childlogdir cannot be blank\n");
				exit_routine();
			}
			env->opt.str[CHILDLOGDIR] = tmp;
		}
	}

/*	if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:umask", NULL)) != NULL)
		env->opt.optmask |= get_nodaemon(tmp);

	if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:environment", NULL)) != NULL)
			env->opt.optmask |= get_nodaemon(tmp);
*/


	tmp = (char *)iniparser_getstring(dict, "taskmasterd:umask", NULL);
	env->opt.umask = (mode_t)get_umask(tmp);

	tmp = (char *)iniparser_getstring(dict, "taskmasterd:environment", NULL);
	env->opt.environ = tmp;

}
