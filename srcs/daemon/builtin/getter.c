/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/04 21:19:08 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 17:38:32 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static t_vector	*exec_one_arg(char *arg, t_action to_do)
{
	char		*name;
	t_program	*prog;
	char		*nb;
	t_vector	*vct;

	name = ft_strchr(arg, ':') != NULL ? ft_strcdup(arg, ':') : NULL;
	vct = NULL;
	if (name != NULL)
	{
		prog = find_program(name, NULL);
		nb = arg + ft_strlen(name) + 1;
		if (nb != NULL && prog != NULL)
		{
			if (ft_strequ(nb, "*") == true)
				vct = exec_wildcard(prog, arg, to_do);
			else if (nb[0] != '\0' && ft_strcheck(nb, ft_isdigit) == true
					&& ft_strlen(nb) < 4 && ft_atoi(nb) < LIMIT_INSTANCE)
				vct = process_instance(prog, ft_atoi(nb), arg, to_do);
		}
	}
	if (vct == NULL)
		vct = get_msg(arg, "no such process", ERR_MSG);
	ft_strdel(&name);
	return (vct);
}

t_vector		*exec_action_args(char **arg, int ac, t_action to_do)
{
	t_vector	*output;
	t_vector	*vct;
	int			i;

	i = 0;
	output = vct_new(0);
	while (i < ac && arg[i] != NULL)
	{
		vct = exec_one_arg(arg[i], to_do);
		vct_cat(output, vct);
		vct_del(&vct);
		i++;
	}
	return (output);
}

t_vector		*exec_action_all(t_action to_do)
{
	t_list		*prog_list;
	t_program	*program;
	t_instance	*instance;
	t_vector	*output;
	t_vector	*vct;

	prog_list = g_denv->prgm_list;
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

t_vector		*exec_action_all_group(t_action to_do)
{
	t_list		*prog_list;
	t_program	*program;
	t_vector	*output;
	t_vector	*vct;

	prog_list = g_denv->prgm_list;
	output = vct_new(0);
	while (prog_list != NULL)
	{
		program = (t_program *)prog_list->content;
		if (program != NULL)
		{
			vct = to_do(NULL, program);
			vct_cat(output, vct);
			vct_del(&vct);
		}
		prog_list = prog_list->next;
	}
	return (output);
}

t_vector		*exec_action_args_group(char **arg, int ac,
					t_action to_do, t_denv *env)
{
	t_program	*program;
	t_vector	*output;
	t_vector	*vct;
	int			i;

	i = 0;
	output = vct_new(0);
	while (i < ac)
	{
		program = find_program(arg[i], env);
		if (program != NULL)
			vct = to_do(NULL, program);
		else
			vct = get_msg(arg[i], "no such group", ERR_MSG);
		vct_cat(output, vct);
		vct_del(&vct);
		i++;
	}
	return (output);
}
