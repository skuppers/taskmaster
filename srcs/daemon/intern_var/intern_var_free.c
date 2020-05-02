/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intern_var_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 19:28:10 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/02 19:28:11 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void			free_node(void *node)
{
	t_variable	*variable;

	variable = (t_variable *)node;
	if (variable != NULL)
	{
		free(variable->name);
		free(variable->data);
	}
}

static int8_t	del_node(t_list *intern, char *name)
{
	t_list	*tmp;

	tmp = intern;
	intern = intern->next;
	while (intern != NULL)
	{
		if (ft_strequ(((t_variable *)intern->content)->name, name) == TRUE)
		{
			tmp->next = intern->next;
			free_node(intern->content);
			free(intern->content);
			free(intern);
			return (SUCCESS);
		}
		tmp = tmp->next;
		intern = intern->next;
	}
	return (FAILURE);
}

int8_t			free_var(t_list **alst, char *name)
{
	t_list		*intern;
	t_variable	*var;

	intern = *alst;
	if (intern == NULL)
		return (SUCCESS);
	var = (t_variable *)intern->content;
	if (ft_strequ(var->name, name) == TRUE)
	{
		*alst = intern->next;
		free_node(intern->content);
		free(intern->content);
		free(intern);
		return (SUCCESS);
	}
	return (del_node(intern, name));
}
