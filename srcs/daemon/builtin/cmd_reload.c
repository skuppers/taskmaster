/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_reload.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:31:57 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 15:50:33 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector			*cmd_reload(t_cmd *cmd)
{
	(void)cmd;
	g_denv->shutdown = S_RELOAD;
	return (vct_newstr("Restarted taskmasterd\n"));
}
