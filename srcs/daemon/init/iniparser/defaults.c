/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaults.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 19:20:31 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

bool		get_nodaemon(char *str)
{
	if (ft_strequ(str, "1") == TRUE || ft_strequ(str, "true") == TRUE)
		return (true);
	return (false);
}

static int	taskmasterd_default_user(dictionary *dict)
{
	char	*tmp;

	if (g_denv->opt.optmask & OPT_USER)
		return (0);
	if ((tmp = (char *)iniparser_getstring(dict,
				"taskmasterd:userid", NULL)) != NULL)
	{
		if (ft_strlen(tmp) <= 0)
			tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd]"
								" - userid cannot be blank.\n");
		else if (ft_strcheck(tmp, ft_isdigit) != TRUE)
			tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd]"
								" - userid has te be numeric.\n");
		else if (ft_strlen(tmp) >= 6 || ft_atoi(tmp) < 0
					|| ft_atoi(tmp) > 16535)
			tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd]"
								" - userid is not in range 0-16535\n");
		else
		{
			g_denv->opt.str[USER] = tmp;
			return (0);
		}
		return (1);
	}
	return (0);
}

static int	taskmasterd_default_directory(dictionary *dict)
{
	char	*tmp;

	if (g_denv->opt.optmask & OPT_DIRECTORY)
		return (0);
	if ((tmp = (char *)iniparser_getstring(dict,
				"taskmasterd:directory", NULL)) != NULL)
	{
		if (ft_strlen(tmp) <= 0)
		{
			tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd]"
								" - directory cannot be blank\n");
			return (1);
		}
		g_denv->opt.str[DIRECTORY] = tmp;
	}
	return (0);
}

static int	taskmasterd_default_logfile(dictionary *dict)
{
	char	*tmp;

	if (g_denv->opt.optmask & OPT_LOGFILE)
		return (0);
	if ((tmp = (char *)iniparser_getstring(dict,
				"taskmasterd:logfile", NULL)) != NULL)
	{
		if (ft_strlen(tmp) <= 0)
		{
			tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd]"
								" - logfile cannot be blank\n");
			return (1);
		}
		g_denv->opt.str[LOGFILE] = tmp;
	}
	return (0);
}

void		taskmasterd_override(dictionary *dict)
{
	int		error;

	error = 0;
	taskmasterd_default_nodaemon(dict);
	error |= taskmasterd_default_user(dict);
	error |= taskmasterd_default_directory(dict);
	error |= taskmasterd_default_logfile(dict);
	error |= taskmasterd_default_loglvl(dict);
	error |= taskmasterd_default_childlogdir(dict);
	error |= taskmasterd_default_umask(dict);
	error |= taskmasterd_default_env(dict);
	if (error == 1)
		exit_routine(E_LOGLVL_ERRO, NULL);
}
