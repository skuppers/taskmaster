/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:35:16 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 13:35:30 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_remove(t_cmd *cmd)
{
	if (cmd->ac == 0)
		return (NULL);
	return (generate_bytecode(cmd, NO_OCP));
}
