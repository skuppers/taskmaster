/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 17:42:50 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 17:43:15 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void			free_node(void *node, size_t content_size)
{
	t_variable	*variable;

	(void)content_size;
	variable = (t_variable *)node;
	if (variable != NULL)
	{
		free(variable->name);
		free(variable->data);
	}
}

static int8_t	del_node(t_list *env, char *name)
{
	t_list	*tmp;

	tmp = env;
	env = env->next;
	while (env != NULL)
	{
		if (ft_strequ(((t_variable *)env->content)->name, name) == TRUE)
		{
			tmp->next = env->next;
			free_node(env->content, sizeof(t_variable));
			free(env->content);
			free(env);
			return (SUCCESS);
		}
		tmp = tmp->next;
		env = env->next;
	}
	return (FAILURE);
}

int8_t			free_var(t_list **alst, char *name)
{
	t_list		*env;
	t_variable	*var;

	env = *alst;
	if (env == NULL)
		return (SUCCESS);
	var = (t_variable *)env->content;
	if (ft_strequ(var->name, name) == TRUE)
	{
		*alst = env->next;
		free_node(env->content, sizeof(t_variable));
		free(env->content);
		free(env);
		return (SUCCESS);
	}
	return (del_node(env, name));
}
