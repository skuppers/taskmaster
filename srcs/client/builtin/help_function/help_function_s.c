/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_function_s.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 18:09:30 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 18:18:21 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void	help_shutdown(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"shutdown", "shut the remote taskmasterd down");
}

void	help_signal(void)
{
	ft_dprintf(STDERR_FILENO, "%-40s%s\n",
		"signal <signal name> <name>", "Signal a process");
	ft_dprintf(STDERR_FILENO, "%-40s%s\n",
		"signal <signal name> <gname>:*",
		"Signal all processes in a group");
	ft_dprintf(STDERR_FILENO, "%-40s%s\n",
		"signal <signal name> <name> <name>",
		"Signal multiple processes or groups");
	ft_dprintf(STDERR_FILENO, "%-40s%s\n",
		"signal <signal name> all", "Signal all processes");
}

void	help_start(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"start <name>", "Start a process");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"start <gname>:*", "Start all processes in a group");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"start <name> <name>", "Start multiple processes or groups");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"start all", "Start all processes");
}

void	help_status(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"status <name>", "Get status for a single process");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"status <gname>:*", "Get status for all processes in a group");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"status <name> <name>", "Get status for multiple processes");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"status", "Get all process status info");
}

void	help_stop(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"stop <name>", "Stop a process");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"stop <gname>:*", "Stop all processes in a group");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"stop <name> <name>", "Stop multiple processes or groups");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"stop all", "Stop all processes");
}
