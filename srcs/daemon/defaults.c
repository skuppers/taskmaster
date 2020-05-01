/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 19:05:55 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

uint8_t get_loglevel(char *str)
{
	if (ft_strequ(str, "debug") == 1)
     	return (DEBG);
	if (ft_strequ(str, "info") == 1)
		return (INFO);
	if (ft_strequ(str, "warning") == 1)
		return (WARN);
	if (ft_strequ(str, "error") == 1)
		return (ERRO);
	if (ft_strequ(str, "critical") == 1)
		return (CRIT);
	return (INFO);
}

uint8_t	get_nodaemon(char *str)
{
	if (ft_strequ(str, "1") == 1 || ft_strequ(str, "true") == 1)
		return (TRUE);
	return (FALSE);
}

uint32_t	get_umask(char *str)
{
	if (ft_strlen(str) > 4)
		return (22);
	if (ft_strcheck(str, ft_isdigit) == FALSE)
		return (22);
	return (ft_atoi(str));
}

char	**get_environ(char *str)
{
	(void)str;
	return (NULL);
}

void    set_taskmasterd_defautls(t_env *env)
{
	env->opt.configfile = DFL_CONFIGURATION;
    env->opt.logfile = DFL_LOGFILE;
    env->opt.loglevel = get_loglevel("info");
    env->opt.nodeamon = FALSE;
    env->opt.umask = 22;  // mode_t bits ; 0x0001x 0x0002w 0x0004r	
	env->opt.user = NULL;
	env->opt.directory = NULL;
	env->opt.childlogdir = DFL_CHLDLOGDIR;
	env->opt.environ = NULL;
}
