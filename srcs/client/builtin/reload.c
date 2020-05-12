/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reload.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:34:42 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 14:37:56 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_reload(t_cmd *cmd)
{
	t_vector *line;
	t_vector *bytecode;

	(void)cmd;
	ft_dprintf(STDERR_FILENO,
			"Really restart the remote taskmasterd process down y/N? ");
	line = vct_new(DFL_VCT_SIZE);
	bytecode = NULL;
	canonic_mode(true);
	if (vct_readline(line, STDIN_FILENO) > 0)
	{
		vct_apply(line, LOWCASE);
		if (ft_strequ("yes", vct_getstr(line)) == true
			|| ft_strequ("y", vct_getstr(line)) == true)
			bytecode = generate_bytecode(cmd, NO_OCP);
	}
	canonic_mode(false);
	vct_del(&line);
	return (bytecode);
}
