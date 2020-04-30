/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:30:34 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 13:30:46 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_add(t_cmd *cmd)
{
	if (cmd->ac != 0)
		return (generate_bytecode(cmd, NO_OCP));
	return (NULL);
}
