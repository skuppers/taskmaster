/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:33:41 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 02:33:52 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_denv				*g_newenv;

t_vector		*update_file(t_instance *in, t_program *prg)
{
	(void)in;(void)prg;
	t_list	*ptr;


	ptr = g_newenv->prgm_list;
	if (ptr == NULL)
		return (vct_newstr("Nothing to update.\n"));

	// program count
	// 
//	g_newenv = NULL;
	return (vct_newstr("Updated config file.\n"));
}

t_vector			*cmd_update(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x01 || cmd->ocp == 0x02)
		vct = update_file(NULL, NULL);
	return (vct);
}
