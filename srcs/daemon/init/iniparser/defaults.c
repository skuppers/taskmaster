/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaults.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/13 17:13:12 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

bool	get_nodaemon(char *str)
{
	if (ft_strequ(str, "1") == TRUE || ft_strequ(str, "true") == TRUE)
		return (true);
	return (false);
}

void	taskmasterd_override(t_denv *env, dictionary *dict)
{
	char	*tmp;
	int		error;

	error = 0;
	if ((env->opt.optmask & OPT_NODAEMON) == FALSE)
	{
		if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:nodaemon", NULL)) != NULL)
			env->opt.optmask |= (get_nodaemon(tmp) == true) ? OPT_NODAEMON : 0;
	}
	if ((env->opt.optmask & OPT_USER) == FALSE)
	{
		if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:userid", NULL)) != NULL)
		{
			if (ft_strlen(tmp) <= 0)
			{
				tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd] - userid cannot be blank.\n");
				error = 1;
			}
			else if (ft_strcheck(tmp, ft_isdigit) != TRUE)
			{
				tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd] - userid has te be numeric.\n");
				error = 1;
			}
			else if (ft_strlen(tmp) >= 6 || ft_atoi(tmp) < 0 || ft_atoi(tmp) > 16535)
			{
				tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd] - userid is not in range 0-16535\n");
				error = 1;
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
				error = 1;
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
				error = 1;
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
				error = 1;
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
				error = 1;
			}
			env->opt.str[CHILDLOGDIR] = tmp;
		}
	}
	if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:umask", NULL)) != NULL)
	{
		if (ft_strlen(tmp) <= 0)
		{
			tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd] - umask field cannot be blank.\n");
			error = 1;
		}
		if (ft_strcheck(tmp, ft_isdigit) != TRUE)
		{
			error = 1;
			dprintf(STDERR_FILENO, "taskmasterd: [taskmasterd] - umask field must be numeric.\n");
		}
		env->opt.umask = strtol(tmp, NULL, 8);
	}
	if ((tmp = (char *)iniparser_getstring(dict, "taskmasterd:environment", NULL)) != NULL)
	{
		if (ft_strlen(tmp) <= 0)
		{
			tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd] - environment field cannot be blank.\n");
			error = 1;
		}
	}
	env->opt.environ = tmp;
	if (error == 1)
		exit_routine(E_LOGLVL_ERRO, NULL);
}
