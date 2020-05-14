/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_starting_debug.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 17:53:40 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 12:46:46 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void	print_prog_debug(t_program *prog)
{
	dprintf(STDERR_FILENO, "Program: %s\n", prog->name);
	dprintf(STDERR_FILENO, " - command:\t%s\n", prog->command);
	dprintf(STDERR_FILENO, " - bin:\t%s\n", prog->bin);
	dprintf(STDERR_FILENO, " - numprocs:\t%d\n", prog->numprocs);
	dprintf(STDERR_FILENO, " - autostart:\t%d\n", prog->autostart);
	dprintf(STDERR_FILENO, " - autorestart:\t%d\n", prog->autorestart);
	dprintf(STDERR_FILENO, " - startsecs:\t%d\n", prog->startsecs);
	dprintf(STDERR_FILENO, " - startretries:\t%d\n", prog->startretries);
	dprintf(STDERR_FILENO, " - stopwaitsec:\t%d\n", prog->stopwaitsecs);
	dprintf(STDERR_FILENO, " - exitcodes:\n");
	ft_print_strtab(prog->exitcodes);
	dprintf(STDERR_FILENO, " - directory:\t%s\n", prog->directory);
	dprintf(STDERR_FILENO, " - umask:\t%u\n", prog->umask);
	dprintf(STDERR_FILENO, " - priority:\t%d\n", prog->priority);
	dprintf(STDERR_FILENO, " - stopsignals:\t%d\n", prog->stopsignal);
	dprintf(STDERR_FILENO, " - user:\t\t%d\n", prog->userid);
	dprintf(STDERR_FILENO, " - stdout_logfile:\t%s\n", prog->stdout_logfile);
	dprintf(STDERR_FILENO, " - stderr_logfile:\t%s\n", prog->stderr_logfile);
	dprintf(STDERR_FILENO, " - environment:\t%s\n\n", prog->environ);
}

void		print_starting_debug(t_denv *env)
{
	t_list *ptr;

	dprintf(STDERR_FILENO, "Taskmasterd:\n");
	dprintf(STDERR_FILENO, "- logfile: %s\n", env->opt.str[LOGFILE]);
	dprintf(STDERR_FILENO, "- loglevel: %s\n", env->opt.str[LOGLEVEL]);
	dprintf(STDERR_FILENO, "- umask: %o\n", env->opt.umask);
	dprintf(STDERR_FILENO, "- userid: %s\n", env->opt.str[USER]);
	dprintf(STDERR_FILENO, "- directory: %s\n", env->opt.str[DIRECTORY]);
	dprintf(STDERR_FILENO, "- childlogdir: %s\n", env->opt.str[CHILDLOGDIR]);
	dprintf(STDERR_FILENO, "- environment: %s\n\n", env->opt.environ);
	ptr = env->prgm_list;
	while (ptr != NULL)
	{
		print_prog_debug((t_program *)ptr->content);
		ptr = ptr->next;
	}
}
