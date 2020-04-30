/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_function_a_f.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 18:05:24 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 18:18:39 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void	help_add(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"add <name> [...]",
		"Activates any updates in config for process/group");
}

void	help_avail(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"avail", "Display all configured processes");
}

void	help_clear(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"clear <name>", "Clear a process' log files");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"clear <name> <name>", "Clear multiple process' log files");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"clear all", "Clear all process' log files");
}

void	help_exit(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"exit", "Exit the taskmaster shell");
}

void	help_fg(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"fg <process>", "Connect to a process in foreground mode");
	ft_dprintf(STDERR_FILENO, "%24c%s\n", ' ',
							"Press Ctrl+C to exit foreground");
}
