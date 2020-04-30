/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 20:20:57 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 18:30:13 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void			del_completion_list(void *mem, size_t content_size)
{
	(void)content_size;
	ft_memdel(&mem);
}

static char		*get_last_word(t_vector *vct)
{
	int		i;

	i = vct_len(vct) == 0 ? 0 : vct_len(vct) - 1;
	if (i == 0)
		return (ft_strdup(vct_getstr(vct)));
	while (i >= 0 && vct_getcharat(vct, i) != ' ')
		i--;
	return (ft_strcdup(vct_getstr(vct) + i + 1, ' '));
}

size_t			get_max_len(size_t len, uint8_t flag)
{
	static size_t	max_len;

	if (flag == REINIT)
		max_len = 0;
	else if (len > max_len)
		max_len = len;
	return (max_len);
}

static int8_t	fill_word(t_vector *vct, t_list *possible_cmd, size_t len)
{
	vct_cutfrom(vct, vct_len(vct) - len);
	vct_addstr(vct, (char *)(possible_cmd->content));
	vct_add(vct, ' ');
	ft_lstdel(&possible_cmd, del_completion_list);
	return (0);
}

int8_t			completion(t_vector *vct)
{
	char		*last_word;
	size_t		len;
	uint8_t		i;
	t_list		*possible_cmd;
	t_list		*node;

	possible_cmd = NULL;
	get_max_len(0, REINIT);
	last_word = get_last_word(vct); // MALLOC PROTECT
	len = ft_strlen(last_word);
	i = 0;
	while (i < NB_CMD)
	{
		if (len == 0 || ft_strnequ(last_word, get_keyword(i), len) == TRUE)
		{
			node = ft_lstnew(get_keyword(i), ft_strlen(get_keyword(i)) + 1);
			ft_lstadd_back(&possible_cmd, node);
			get_max_len(ft_strlen(get_keyword(i)), SET);
		}
		i++;
	}
	ft_strdel(&last_word);
	if (possible_cmd != NULL && possible_cmd->next == NULL)
		return (fill_word(vct, possible_cmd, len));
	return (print_completion(possible_cmd));
}
