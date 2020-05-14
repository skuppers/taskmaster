/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_status_completion.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 16:00:27 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 16:03:41 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void			insert_prog_name(t_vector *output, t_list *prog_list,
						t_program *program)
{
	if (program->numprocs > 0)
	{
		if (prog_list != g_denv->prgm_list)
			vct_add(output, '/');
		vct_addstr(output, program->name);
		vct_add(output, '/');
		vct_addstr(output, program->name);
		vct_addstr(output, ":*;");
	}
}

t_vector			*progs_info(t_list *prog_list)
{
	t_program	*program;
	t_instance	*instance;
	t_vector	*output;

	output = vct_new(0);
	while (prog_list != NULL)
	{
		program = (t_program *)prog_list->content;
		if (program != NULL)
		{
			insert_prog_name(output, prog_list, program);
			instance = program->instance;
			while (instance != NULL)
			{
				vct_addstr(output, instance->name);
				instance = instance->next;
				if (instance != NULL)
					vct_add(output, ';');
			}
		}
		prog_list = prog_list->next;
	}
	return (output);
}
