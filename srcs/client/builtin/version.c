/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   version.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 13:08:27 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 13:28:50 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_version(t_cmd *cmd)
{
	return (generate_bytecode(cmd, NO_OCP));
}

