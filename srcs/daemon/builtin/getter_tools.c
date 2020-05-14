/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 16:15:27 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 16:19:21 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*get_msg(char *name, char *msg, uint8_t flag)
{
	t_vector	*vct;

	vct = vct_newstr(name);
	if (vct != NULL)
	{
		if (flag == ERR_MSG)
		{
			vct_addstr(vct, ": ERROR (");
			vct_addstr(vct, msg);
			vct_addstr(vct, ")\n");
		}
		else
		{
			vct_addstr(vct, ": ");
			vct_addstr(vct, msg);
			vct_addstr(vct, "\n");
		}
	}
	return (vct);
}

t_vector	*process_instance(t_program *program, uint8_t nb_instance,
							char *arg, t_action to_do)
{
	t_instance	*instance;

	if (program != NULL)
	{
		instance = get_instance(program, nb_instance);
		if (instance != NULL)
			return (to_do(instance, program));
	}
	return (get_msg(arg, "no such process", ERR_MSG));
}

t_program	*find_program(const char *name, t_denv *e)
{
	t_list		*prog_list;
	t_program	*program;
	t_denv		*env;

	env = g_denv;
	if (e != NULL)
		env = e;
	prog_list = env->prgm_list;
	while (prog_list != NULL)
	{
		program = (t_program *)prog_list->content;
		if (program != NULL && program->name != NULL
			&& ft_strequ(program->name, name) == TRUE)
			return (program);
		prog_list = prog_list->next;
	}
	return (NULL);
}

t_vector	*exec_wildcard(t_program *prog, char *arg, t_action to_do)
{
	t_vector	*output;
	t_vector	*vct;
	int			i;

	i = 0;
	output = vct_new(0);
	while (i < prog->numprocs)
	{
		vct = process_instance(prog, i, arg, to_do);
		vct_cat(output, vct);
		vct_del(&vct);
		i++;
	}
	return (output);
}
