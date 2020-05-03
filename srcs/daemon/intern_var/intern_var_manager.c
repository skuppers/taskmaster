/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intern_var_manager.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 19:05:15 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 15:27:40 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_list	*envtolst(char **tab)
{
	t_list		*lst;
	t_variable	variable;
	t_list		*tmp;
	int			i;

	lst = NULL;
	i = 0;
	while (tab[i])
		i++;
	while (--i >= 0)
	{
		variable.name = ft_strcdup(tab[i], '=');
		variable.data = (variable.name == NULL) ?
				NULL : ft_strdup(tab[i] + ft_strlen(variable.name));
		if ((tmp = ft_lstnew(&variable, sizeof(t_variable))) == NULL)
		{
			ft_lstdel(&lst, NULL);
			return (NULL);
		}
		ft_lstadd_back(&lst, tmp);
	}
	return (lst);
}

char	**envtotab(t_list *lst)
{
	char		**result;
	t_variable	*variable;
	size_t		listsize;
	size_t		index;

	listsize = ft_lstsize(lst);
	result = (char **)malloc(sizeof(char *) * (listsize + 1));
	index = 0;
	while (index < listsize)
	{
		variable = (t_variable *)lst->content;
		result[index] = ft_asprintf("%s=%s", variable->name,
				variable->data == NULL ? "" : variable->data);
		index++;
		lst = lst->next;
	}
	result[index] = NULL;
	return (result);
}

void			print_lst(t_list *lst)
{
	t_variable	*variable;

	while (lst != NULL)
	{
		variable = (t_variable *)lst->content;
		if (variable->data != NULL)
			ft_dprintf(STDERR_FILENO, "%s=%s\n",
						variable->name, variable->data);
		else
			ft_dprintf(STDERR_FILENO, "%s=\n", variable->name);
		lst = lst->next;
	}
}

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

char		*get_var(t_list *intern, char *name)
{
	t_list		*node;

	if ((node = ft_lstfind(intern, name, find_var)))
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

int8_t			add_var_vct(t_list **alst, t_vector *val)
{
	t_vector	*name;

	if (val == NULL) 
		return (FAILURE);
	vct_trimfront(val, " \t");
	if (vct_apply(val, IS_PRINTABLE) == FALSE || vct_getfirstchar(val) == '='
		|| vct_len(val) == 0)
		return (FAILURE);
	name = vct_splitchr(val, '=', DEL_CHAR);
	vct_trimfront(val, " \t");
	if ((vct_getfirstchar(val) == '"' && vct_getlastchar(val) == '"')
		|| (vct_getfirstchar(val) == '\'' && vct_getlastchar(val) == '\''))
	{
		vct_pop(val);
		vct_cut(val);
	}
	add_var(alst, vct_getstr(name), vct_getstr(val));
	vct_del(&name);
	return (SUCCESS);
}

void	strvalue_to_lst(t_list **lst, char *str)
{
	t_vector	*vct;
	t_vector	*split;

	vct = vct_newstr(str);
	while ((split = vct_split(vct, DELIMITER_STR, NO_SEP)) != NULL)
	{
		add_var_vct(lst, split);
		vct_del(&split);
	}
	vct_del(&vct);
}

