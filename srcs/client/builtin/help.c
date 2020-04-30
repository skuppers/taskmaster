/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 16:10:20 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 17:56:16 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void	help_add(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"add <name> [...]",
		"Activates any updates in config for process/group");
}

void	help_avail(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"avail", "Display all configured processes");
}

void	help_clear(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"clear <name>", "Clear a process' log files");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"clear <name> <name>", "Clear multiple process' log files");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"clear all", "Clear all process' log files");
}

void	help_exit(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"exit", "Exit the taskmaster shell");
}

void	help_fg(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"fg <process>", "Connect to a process in foreground mode");
	ft_dprintf(STDERR_FILENO,"%24c%s\n", ' ',
							"Press Ctrl+C to exit foreground");
}

void	help_help(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"help", "Print a list of available actions");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"help <action>", "Print help for <action>");
}

void	help_maintail(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"maintail -f", "Continuous tail of taskmaster main log file "
		"(Ctr+C to exit)" );
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"maintail -100", "Last 100 *bytes* of taskmaster main log file");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"maintail", "Last 1600 *bytes* of taskmaster main log file");
}

void	help_open(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s%24c%s\n",
		"open <url>", "Connect to a remote taskmaster process\n", ' ',
						"(for UNIX domain socket, use unix:///socket/path)");
}

void	help_pid(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"pid", "Get the PID of taskmasterd");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"pid <name>", "Get the PID of a single child process by name");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"pid all", "Get the PID of every child process, one per line");
}

void	help_quit(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"quit", "Exit the taskmaster shell");
}

void	help_reload(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"reload", "Restart the remote taskmasterd");
}

void	help_remove(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"remove <name> [...]",
		"Remove process/group from active config");
}

void	help_reread(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"reread", "Reload the daemon's configuration files");
}

void	help_restart(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"restart <name>", "Restart a process");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"restart <gname>:*", "Restart all processes in a group");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"restart <name> <name>", "Restart multiple processes or groups");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"restart all", "Restart all processes");
	ft_dprintf(STDERR_FILENO, "Note: restart does not reread config files."
								" For that, see reread and update.\n");
}

void	help_shutdown(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"shutdown", "shut the remote taskmasterd down");
}

void	help_signal(void)
{
	ft_dprintf(STDERR_FILENO,"%-40s%s\n",
		"signal <signal name> <name>", "Signal a process");
	ft_dprintf(STDERR_FILENO,"%-40s%s\n",
		"signal <signal name> <gname>:*",
		"Signal all processes in a group");
	ft_dprintf(STDERR_FILENO,"%-40s%s\n",
		"signal <signal name> <name> <name>",
		"Signal multiple processes or groups");
	ft_dprintf(STDERR_FILENO,"%-40s%s\n",
		"signal <signal name> all", "Signal all processes");
}

void	help_start(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"start <name>", "Start a process");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"start <gname>:*", "Start all processes in a group");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"start <name> <name>", "Start multiple processes or groups");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"start all", "Start all processes");
}

void	help_status(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"status <name>", "Get status for a single process");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"status <gname>:*", "Get status for all processes in a group");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"status <name> <name>", "Get status for multiple processes");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"status", "Get all process status info");
}

void	help_stop(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"stop <name>", "Stop a process");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"stop <gname>:*", "Stop all processes in a group");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"stop <name> <name>", "Stop multiple processes or groups");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"stop all", "Stop all processes");
}

void	help_tail(void)
{
	ft_dprintf(STDERR_FILENO,
		"tail [-f] <name> [stdout|stderr] (default stdout)\nEx:\n");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"tail -f <name>", "Continuous tail of named process stdout "
		"(Ctr+C to exit)");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"tail -100 <name>", "Last 100 *bytes* of process stdout");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"tail <name> stderr", "Last 1600 *bytes* of process stderr");
}

void	help_update(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"update", "Reload config and add/remove as necessary");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"update all", "Reload config and add/remove as necessary");
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"update <gname> [...]", "Update specific groups");
}

void	help_version(void)
{
	ft_dprintf(STDERR_FILENO,"%-24s%s\n",
		"version", "Show the version of the remote taskmasterd process");
}

static void	ask_help(t_cmd *cmd)
{
	static t_help	ft_help[] = {help_add, help_avail, help_clear, help_exit,
								help_fg, help_help, help_maintail, help_open,
								help_pid, help_quit, help_reload, help_remove,
								help_reread, help_restart, help_shutdown,
								help_signal, help_start, help_status, help_stop,
								help_tail, help_update, help_version};
	uint8_t	i;


	i = 0;
	while (i < NB_CMD)
	{
		if (ft_strequ(cmd->av[0], get_keyword(i)) == TRUE)
		{
			ft_help[i]();
			return ;
		}
		i++;
	}
	ft_dprintf(STDERR_FILENO, "*** No Help on %s\n", cmd->av[0]);
}

t_vector	*blt_help(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "\ndefault commands (type help <topic>):\n");
		ft_dprintf(STDERR_FILENO, "=====================================\n");
		ft_dprintf(STDERR_FILENO,
				"add\texit\t  open\treload\trestart\t  start\t  tail\n");
		ft_dprintf(STDERR_FILENO,
				"avail\tfg\t  pid\tremove\tshutdown  status  update\n");
		ft_dprintf(STDERR_FILENO,
				"clear\tmaintail  quit\treread\tsignal\t  stop\t  version\n\n");
	}
	else
		ask_help(cmd);
	return (NULL);
}
