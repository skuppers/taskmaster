/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:31:47 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 13:32:00 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_open(t_cmd *cmd)
{
	if (cmd->ac == 0)
	{
		ft_dprintf(STDERR_FILENO, "Error: url must be http:// or unix://\n");
		return (NULL);
	}
	return (generate_bytecode(cmd, NO_OCP));
}
