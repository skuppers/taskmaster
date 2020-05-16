/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 17:42:27 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/16 11:42:56 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

/*
**	void			print_lst(t_list *lst)
**	{
**		t_variable	*variable;
**
**		while (lst != NULL)
**		{
**			variable = (t_variable *)lst->content;
**			if (variable->data != NULL)
**				dprintf(STDERR_FILENO, "%s=%s\n",
**							variable->name, variable->data);
**			else
**				dprintf(STDERR_FILENO, "%s=\n", variable->name);
**			lst = lst->next;
**		}
**	}
*/

void			concat_env_to_daemon_env(t_program *prog, t_list *env)
{
	t_variable	*var;

	while (env != NULL)
	{
		var = (t_variable *)(env->content);
		if (var != NULL && get_var(prog->env, var->name) == NULL)
			add_var(&prog->env, var->name, var->data);
		env = env->next;
	}
}

char			**envtotab(t_list *lst)
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

t_list			*envtolst(char **tab)
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
				NULL : ft_strdup(tab[i] + ft_strlen(variable.name) + 1);
		if ((tmp = ft_lstnew(&variable, sizeof(t_variable))) == NULL)
		{
			ft_lstdel(&lst, NULL);
			return (NULL);
		}
		ft_lstadd_back(&lst, tmp);
	}
	return (lst);
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

void			strvalue_to_lst(t_list **lst, char *str)
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
