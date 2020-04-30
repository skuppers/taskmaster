/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_function_r.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 18:08:57 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 18:19:10 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void	help_reload(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"reload", "Restart the remote taskmasterd");
}

void	help_remove(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"remove <name> [...]",
		"Remove process/group from active config");
}

void	help_reread(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"reread", "Reload the daemon's configuration files");
}

void	help_restart(void)
{
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"restart <name>", "Restart a process");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"restart <gname>:*", "Restart all processes in a group");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"restart <name> <name>", "Restart multiple processes or groups");
	ft_dprintf(STDERR_FILENO, "%-24s%s\n",
		"restart all", "Restart all processes");
	ft_dprintf(STDERR_FILENO, "Note: restart does not reread config files."
								" For that, see reread and update.\n");
}
