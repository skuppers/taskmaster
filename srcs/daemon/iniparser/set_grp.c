/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_grp.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 17:38:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 17:47:51 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static t_list	*get_prgm_node(t_list *prog_list, char *name)
{
	char	*node_name;

	while (prog_list != NULL)
	{
		if (prog_list->content != NULL)
		{
			node_name = (char *)(((t_program *)(prog_list->content))->name);
			if (node_name != NULL)
			{
				if (ft_strequ(node_name, name) == TRUE)
					return (ft_lstnew_nomalloc(prog_list->content,
						sizeof(t_program)));
			}
		}
		prog_list = prog_list->next;
	}
	return (NULL);
}

void			set_grp_list(t_env *env)
{
	t_vector	*vct;
	t_vector	*split;
	t_list		*lst_grp;
	t_group		*cur_grp;
	t_list		*prgm_node;

	lst_grp = env->group_list;
	while (lst_grp != NULL)
	{
		cur_grp = (t_group *)(lst_grp->content);
		if (cur_grp != NULL && cur_grp->programs != NULL)
		{
			vct = vct_newstr(cur_grp->programs);
			cur_grp->prog_list = NULL;
			while ((split = vct_split(vct, DELIMITER_STR, NO_SEP)) != NULL)
			{
				vct_trim(split, "\t ");
				prgm_node = get_prgm_node(env->prgm_list, vct_getstr(split));
				if (prgm_node == NULL)
					 ft_dprintf(STDERR_FILENO,
							"prog %s is not set in conf file\n",
							vct_getstr(split));// print dans debug
				else
					ft_lstadd(&cur_grp->prog_list, prgm_node);
				vct_del(&split);
			}
			vct_del(&vct);
		}
		lst_grp = lst_grp->next;
	}
/*	/// DEBUG //////////////

	lst_grp = env->group_list;
	while (lst_grp != NULL)
	{
		cur_grp = (t_group *)(lst_grp->content);
		ft_printf("----- GROUP `%s' -----\n", cur_grp->name);
		for (t_list *tmp = cur_grp->prog_list; tmp != NULL; tmp = tmp->next)
		{
			char *name = (tmp->content == NULL) ? ""
				: ((t_program *)(tmp->content))->name;
			ft_printf("\t+--> prog: %s\n", name);
		}
		lst_grp = lst_grp->next;
	}*/
}
