/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_shutdown.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:33:17 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 15:41:52 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector			*cmd_shutdown(t_cmd *cmd)
{
	(void)cmd;
	g_denv->shutdown = S_SHUTDOWN;
	return (vct_newstr("Shut down\n"));
}
