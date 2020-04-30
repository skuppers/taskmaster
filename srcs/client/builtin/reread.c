/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:33:52 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 13:33:54 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_reread(t_cmd *cmd)
{
	return (generate_bytecode(cmd, NO_OCP));
}
