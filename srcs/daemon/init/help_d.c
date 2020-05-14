/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 16:23:46 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 16:23:55 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void	print_version(void)
{
	dprintf(STDERR_FILENO, "Taskmasterd v0.1\n");
}

void	print_help(void)
{
	dprintf(STDERR_FILENO,
		"taskmasterd -- run a set of applications as daemons.\n"
		"Usage: /usr/bin/taskmasterd [options]\n"
		"Options:\n"
		"-c/--configuration FILENAME -- configuration"
		" file path (searches if not given)\n"
		"-n/--nodaemon -- run in the foreground"
		" (same as 'nodaemon=true' in config file)\n"
		"-h/--help -- print this usage message and exit\n"
		"-v/--version -- print taskmasterd version number and exit\n"
		"-u/--user USER -- run taskmasterd as this user (or numeric uid)\n"
		"-m/--umask UMASK --"
		" use this umask for daemon subprocess (default is 022)\n"
		"-d/--directory DIRECTORY -- directory to chdir to when daemonized\n"
		"-l/--logfile FILENAME -- use FILENAME as logfile path\n"
		"-e/--loglevel LEVEL"
		" -- use LEVEL as log level (debug,info,warn,error,critical)\n"
		"-q/--childlogdir DIRECTORY"
		" -- the log directory for child process logs\n"
		"                   old automatic child log files) at startup.\n");
}
