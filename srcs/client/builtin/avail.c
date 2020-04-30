/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   avail.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:30:54 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 18:21:43 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_avail(t_cmd *cmd)
{
	return (generate_bytecode(cmd, NO_OCP));
}
