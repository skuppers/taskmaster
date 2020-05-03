/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 17:42:34 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 17:42:44 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

int			find_var(void *data, void *to_find)
{
	t_variable	*variable;
	char		*name;

	name = to_find;
	variable = data;
	return (ft_strequ(variable->name, name));
}

t_list	*ft_lstfind(t_list *lst, void *to_find, int (*f)(void *, void *))
{
	if (lst == NULL)
		return (NULL);
	return (f(lst->content, to_find) ? lst : ft_lstfind(lst->next, to_find, f));
}

char		*get_var(t_list *env, char *name)
{
	t_list		*node;

	if ((node = ft_lstfind(env, name, find_var)))
		return (((t_variable *)node->content)->data);
	return (NULL);
}

static int8_t			create_node(t_list **alst, char *name, char *data)
{
	t_variable	variable;
	t_list		*newdir;

	if (name == NULL)
		return (FAILURE);
	ft_bzero(&variable, sizeof(t_variable));
	variable.name = ft_strdup(name);
	variable.data = ft_strdup(data);
	if (variable.name == NULL)
		return (FAILURE);
	if ((newdir = ft_lstnew(&variable, sizeof(t_variable))) == NULL)
		return (FAILURE);
	ft_lstadd_back(alst, newdir);
	return (SUCCESS);
}

int8_t			add_var(t_list **alst, char *name, char *data)
{
	t_list *lst;

	lst = *alst;
	if (name == NULL)
		return (FAILURE);
	while (lst != NULL)
	{
		if (ft_strequ(((t_variable *)lst->content)->name, name) == TRUE)
		{
			ft_strdel(&(((t_variable *)lst->content)->data));
			((t_variable *)lst->content)->data = ft_strdup(data);
			return (SUCCESS);
		}
		lst = lst->next;
	}
	return (create_node(alst, name, data));
}
