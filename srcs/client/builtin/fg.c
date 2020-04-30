/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fg.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:32:32 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 13:32:38 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_fg(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: no process name supplied\n");
		// call "help clear"
		return (NULL);
	}
	else if (cmd->ac > 1)
	{
		ft_dprintf(STDERR_FILENO, "Error: too many process names supplied\n");
		return (NULL);
	}
	return (generate_bytecode(cmd, NO_OCP));
}
