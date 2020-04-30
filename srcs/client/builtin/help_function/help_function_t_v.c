/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_function_t_v.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 18:10:36 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 18:19:30 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void	help_tail(void)
{
	ft_dprintf(STDERR_FILENO,
		"tail [-f] <name> [stdout|stderr] (default stdout)\nEx:\n");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"tail -f <name>", "Continuous tail of named process stdout "
		"(Ctr+C to exit)");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"tail -100 <name>", "Last 100 *bytes* of process stdout");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"tail <name> stderr", "Last 1600 *bytes* of process stderr");
}

void	help_update(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"update", "Reload config and add/remove as necessary");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"update all", "Reload config and add/remove as necessary");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"update <gname> [...]", "Update specific groups");
}

void	help_version(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"version", "Show the version of the remote taskmasterd process");
}
