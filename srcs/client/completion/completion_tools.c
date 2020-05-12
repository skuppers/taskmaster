/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion_tools.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/11 19:10:12 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 02:44:49 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void			del_completion_list(void *mem, size_t content_size)
{
	(void)content_size;
	ft_memdel(&mem);
}

char			*get_last_word(t_vector *vct)
{
	int		i;

	i = vct_len(vct) == 0 ? 0 : vct_len(vct) - 1;
	if (i == 0)
		return (ft_strdup(vct_getstr(vct)));
	while (i >= 0 && vct_getcharat(vct, i) != ' '
			&& vct_getcharat(vct, i) != ';')
		i--;
	if (ft_strclen(vct_getstr(vct), ' ') < ft_strclen(vct_getstr(vct), ';'))
		return (ft_strcdup(vct_getstr(vct) + i + 1, ' '));
	return (ft_strcdup(vct_getstr(vct) + i + 1, ';'));
}

size_t			get_max_len(const size_t len, const uint8_t flag)
{
	static size_t	max_len;

	if (flag == REINIT)
		max_len = 0;
	else if (len > max_len)
		max_len = len;
	return (max_len);
}

void			lst_add_node(t_list **list, const char *str)
{
	t_list	*node;

	node = ft_lstnew(str, ft_strlen(str) + 1);
	if (node != NULL)
		ft_lstadd_back(list, node);
	get_max_len(ft_strlen(str), SET);
}
