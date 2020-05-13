/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/04 21:19:08 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 15:03:48 by ffoissey         ###   ########.fr       */
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

t_program *find_program(const char *name, t_denv *e)
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

t_vector	*exec_one_arg(char *arg, t_action to_do)
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
					&& ft_strlen(nb) < 4 && ft_atoi(nb) < 256) // LIMIT INSTANCE ?
				vct = process_instance(prog, ft_atoi(nb), arg, to_do);
		}
	}
	if (vct == NULL)
		vct = get_msg(arg, "no such process", ERR_MSG);
	ft_strdel(&name);
	return (vct);
}

t_vector	*exec_action_args(char **arg, int ac, t_action to_do)
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

t_vector	*exec_action_all(t_action to_do)
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

t_vector	*exec_action_all_group(t_action to_do)
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

t_vector	*exec_action_args_group(char **arg, int ac,
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
