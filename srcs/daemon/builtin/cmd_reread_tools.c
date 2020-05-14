/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_reread_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 17:31:21 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 17:36:33 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

int				do_strcmp(const char *s1, const char *s2)
{
	size_t		i;

	i = 0;
	if (s1 == NULL && s2 != NULL)
		return (1);
	if (s1 != NULL && s2 == NULL)
		return (1);
	if (s1 == NULL && s2 == NULL)
		return (0);
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((int)((unsigned char)s1[i] - (unsigned char)s2[i]));
}

int32_t			compare_prog(t_program *op, t_program *np)
{
	int diff;

	diff = 0;
	diff += do_strcmp(op->command, np->command);
	diff += do_strcmp(op->directory, np->directory);
	diff += do_strcmp(op->stdout_logfile, np->stdout_logfile);
	diff += do_strcmp(op->stderr_logfile, np->stderr_logfile);
	diff += do_strcmp(op->environ, np->environ);
	diff += (op->userid != np->userid) ? 1 : 0;
	diff += (op->umask != np->umask) ? 1 : 0;
	diff += (op->priority != np->priority) ? 1 : 0;
	diff += (op->startsecs != np->startsecs) ? 1 : 0;
	diff += (op->stopwaitsecs != np->stopwaitsecs) ? 1 : 0;
	diff += (op->numprocs != np->numprocs) ? 1 : 0;
	diff += (op->stopsignal != np->stopsignal) ? 1 : 0;
	diff += (op->autostart != np->autostart) ? 1 : 0;
	diff += (op->startretries != np->startretries) ? 1 : 0;
	diff += (op->autorestart != np->autorestart) ? 1 : 0;
	diff += (exitcode_changed(op, np)) ? 1 : 0;
	return (diff);
}

uint8_t			exitcode_changed(t_program *op, t_program *np)
{
	int	i;

	i = 0;
	while (op->exitcodes[i] != NULL)
	{
		if (ft_strequ(op->exitcodes[i], np->exitcodes[i]) == FALSE)
			return (1);
		++i;
	}
	return (0);
}

dictionary		*load_dict(void)
{
	dictionary	*dict;

	dict = iniparser_load(g_denv->opt.str[CONFIGURATION]);
	if (dict == NULL)
		return (NULL);
	g_tmpenv->dict = dict;
	return (dict);
}
