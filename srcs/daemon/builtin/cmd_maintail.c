/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_maintail.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:27:30 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 20:31:00 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector			*cmd_maintail(t_cmd *cmd)
{
	(void)cmd;
	return (vct_newstr(g_denv->opt.str[LOGFILE]));
}
