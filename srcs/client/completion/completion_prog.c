/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion_prog.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/11 19:04:14 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 18:27:33 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static void		fill_prog_name(t_list **list, char *last_word,
						t_vector *vct)
{
	if (ft_strlen(last_word) == 0 || ft_strnequ(last_word, vct_getstr(vct),
						ft_strlen(last_word)) == true)
		if (is_node_uniq(*list, vct_getstr(vct)) == true)
			lst_add_node(list, vct_getstr(vct));
}

static void		fill_instance_name(t_list **list, char *last_word,
						t_vector *vct)
{
	char	**tab;
	uint8_t	i;

	tab = ft_strsplit(vct_getstr(vct), ';');
	i = 0;
	if (tab != NULL)
	{
		while (tab[i] != NULL)
		{
			if (ft_strlen(last_word) == 0 || ft_strnequ(last_word, tab[i],
							ft_strlen(last_word)) == true)
				if (is_node_uniq(*list, tab[i]) == true)
					lst_add_node(list, tab[i]);
			i++;
		}
	}
	ft_free_tab_str(tab);
}

static t_list	*get_prog_list(char *last_word, uint8_t mod, t_fill_list fill)
{
	uint8_t		i;
	t_list		*list;
	t_vector	*feedback;
	t_vector	*split;

	list = NULL;
	feedback = get_status(KEEP_FEEDBACK);
	if (feedback == NULL)
		return (NULL);
	i = 0;
	while ((split = vct_split(feedback, "/", NO_SEP)) != NULL)
	{
		if (i % 2 == mod)
			fill(&list, last_word, split);
		vct_del(&split);
		i++;
	}
	vct_del(&feedback);
	return (list);
}

t_list			*prog_comp(char *last_word)
{
	return (get_prog_list(last_word, 0, fill_prog_name));
}

t_list			*instance_comp(char *last_word)
{
	return (get_prog_list(last_word, 1, fill_instance_name));
}
