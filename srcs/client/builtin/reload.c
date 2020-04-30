/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reload.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:34:42 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 18:23:12 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_reload(t_cmd *cmd)
{
	t_vector *line;

	ft_dprintf(STDERR_FILENO,
			"Really restart the remote taskmasterd process down y/N? ");
	line = vct_new(DFL_VCT_SIZE);
	if (tsk_readline(line, STDIN_FILENO, g_env) > 0)
	{
		vct_apply(line, LOWCASE);
		if (ft_strequ("y", vct_getstr(line)) == TRUE)
		{
			vct_del(&line);
			return (generate_bytecode(cmd, NO_OCP));
		}
	}
	vct_del(&line);
	return (NULL);
}
