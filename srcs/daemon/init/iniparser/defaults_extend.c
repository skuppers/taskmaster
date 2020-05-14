/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   defaults_extend.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 19:12:43 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 19:15:42 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

int		taskmasterd_default_loglvl(dictionary *dict)
{
	char	*tmp;

	if (g_denv->opt.optmask & OPT_LOGLVL)
		return (0);
	if ((tmp = (char *)iniparser_getstring(dict,
				"taskmasterd:loglevel", NULL)) != NULL)
	{
		if (ft_strlen(tmp) <= 0)
		{
			tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd]"
								" - logfile cannot be blank\n");
			return (1);
		}
		g_denv->opt.str[LOGLEVEL] = tmp;
	}
	return (0);
}

int		taskmasterd_default_childlogdir(dictionary *dict)
{
	char	*tmp;

	if (g_denv->opt.optmask & OPT_CHLDLOGDIR)
		return (0);
	if ((tmp = (char *)iniparser_getstring(dict,
				"taskmasterd:childlogdir", NULL)) != NULL)
	{
		if (ft_strlen(tmp) <= 0)
		{
			tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd]"
								" - logfile cannot be blank\n");
			return (1);
		}
		g_denv->opt.str[CHILDLOGDIR] = tmp;
	}
	return (0);
}

void	taskmasterd_default_nodaemon(dictionary *dict)
{
	char	*tmp;

	if (g_denv->opt.optmask & OPT_NODAEMON)
		return ;
	if ((tmp = (char *)iniparser_getstring(dict,
			"taskmasterd:nodaemon", NULL)) != NULL)
		g_denv->opt.optmask |= (get_nodaemon(tmp) == true) ? OPT_NODAEMON : 0;
}

int		taskmasterd_default_umask(dictionary *dict)
{
	char	*tmp;

	if ((tmp = (char *)iniparser_getstring(dict,
				"taskmasterd:umask", NULL)) != NULL)
	{
		if (ft_strlen(tmp) <= 0)
		{
			tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd]"
								" - umask field cannot be blank.\n");
			return (1);
		}
		if (ft_strcheck(tmp, ft_isdigit) != TRUE)
		{
			tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd]"
									" - umask field must be numeric.\n");
			return (1);
		}
		g_denv->opt.umask = strtol(tmp, NULL, 8);
	}
	return (0);
}

int		taskmasterd_default_env(dictionary *dict)
{
	char	*tmp;

	if ((tmp = (char *)iniparser_getstring(dict,
				"taskmasterd:environment", NULL)) != NULL)
	{
		if (ft_strlen(tmp) <= 0)
		{
			tlog(E_LOGLVL_CRIT, "taskmasterd: [taskmasterd]"
								" - environment field cannot be blank.\n");
			return (1);
		}
	}
	g_denv->opt.environ = tmp;
	return (0);
}
