/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shutdown.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 13:35:52 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 13:08:29 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_vector	*blt_shutdown(t_cmd *cmd)
{
	t_vector *line;

	(void)cmd;
	ft_dprintf(STDERR_FILENO,
			"Really shut the remote taskmasterd process down y/N? ");
	line = vct_new(DFL_VCT_SIZE);
	if (tsk_readline(line, STDIN_FILENO, g_env) > 0)
	{
		vct_apply(line, LOWCASE);
		if (ft_strequ("y", vct_getstr(line)) == true)
		{
			vct_del(&line);
			return (generate_bytecode(cmd, NO_OCP));
		}
	}
	vct_del(&line);
	return (NULL);
}
