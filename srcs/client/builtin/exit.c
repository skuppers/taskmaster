/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:29:22 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 13:29:56 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_exit(t_cmd *cmd)                          // A FAIRE
{
	/// call exit routine
	exit(0);
	(void)cmd;
	return (NULL);
}

