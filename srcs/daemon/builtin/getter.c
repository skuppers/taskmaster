/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/04 21:19:08 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/04 22:53:34 by ffoissey         ###   ########.fr       */
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

t_vector	 *process_instance(t_program *program, uint8_t nb_instance,
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

t_program *find_program(const char *name)
{
	t_list		*prog_list;
	t_program	*program;


	prog_list = g_env->prgm_list;
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

t_vector	*exec_action_args(char **arg, int ac, t_action to_do)
{
	char		*name;
	t_vector	*output;
	t_vector	*vct;
	char		*str_nb;
	uint8_t		i;

	i = 0;
	output = vct_new(0);
	while (i < ac && arg[i] != NULL)
	{
		name = ft_strchr(arg[i], ':') != NULL ? ft_strcdup(arg[i], ':') : NULL;
		str_nb = name == NULL ? NULL : ft_strdup(arg[i] + ft_strlen(name) + 1);
		if (name != NULL && str_nb != NULL
			&& ft_strcheck(str_nb, ft_isdigit) == TRUE
			&& ft_strlen(str_nb) < 4 && ft_atoi(str_nb) < 256) // LIMIT INSTANCE
			vct = process_instance(find_program(name),
					(uint8_t)ft_atoi(str_nb), arg[i], to_do); 
		else
			vct = get_msg(arg[i], "no such process", ERR_MSG);
		vct_cat(output, vct);
		vct_del(&vct);
		ft_strdel(&str_nb);
		ft_strdel(&name);
		i++;
	}
	return (output);
}

t_vector	*exec_action_all(t_action to_do)
{
	t_list		*prog_list;
	t_program	*program;
	t_instance	*instance;
	t_vector	*output;
	t_vector	*vct;

	prog_list = g_env->prgm_list;
	output = vct_new(0);
	while (prog_list != NULL)
	{
		program = (t_program *)prog_list->content;
		if (program != NULL)
		{
			instance = program->instance;
			while (instance != NULL)
			{
				vct = to_do(instance, program);
				vct_cat(output, vct);
				vct_del(&vct);
				instance = instance->next;
			}
		}
		prog_list = prog_list->next;
	}
	return (output);
}
