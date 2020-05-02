/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_function_h_q.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 18:08:20 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/02 18:26:21 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void	help_help(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"help", "Print a list of available actions");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"help <action>", "Print help for <action>");
}

void	help_maintail(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"maintail -f", "Continuous tail of taskmaster main log file "
		"(Ctr+C to exit)");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"maintail -100", "Last 100 *bytes* of taskmaster main log file");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"maintail", "Last 1600 *bytes* of taskmaster main log file");
}

void	help_open(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s%24c%s\n",
		"open <url>", "Connect to a remote taskmaster process\n", ' ',
						"(use unix:///socket/path)");
}

void	help_pid(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"pid", "Get the PID of taskmasterd");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"pid <name>", "Get the PID of a single child process by name");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"pid all", "Get the PID of every child process, one per line");
}

void	help_quit(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"quit", "Exit the taskmaster shell");
}
