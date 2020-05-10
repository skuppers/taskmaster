/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 20:20:57 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 12:10:28 by ffoissey         ###   ########.fr       */
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

static int8_t	process_completion(t_vector *vct, t_list *possible_cmd,
					size_t len)
{
	if (ft_lstsize(possible_cmd) == 1)
	{
		vct_cutfrom(vct, vct_len(vct) - len);
		vct_addstr(vct, (char *)(possible_cmd->content));
		vct_add(vct, ' ');
		ft_lstdel(&possible_cmd, del_completion_list);
		return (0);
	}
	return (print_completion(possible_cmd));
}

int8_t			completion(t_vector *vct)
{
	t_list		*possible_cmd;
	char		*last_word;
	t_list		*node;
	size_t		len;
	uint8_t		i;

	i = 0;
	possible_cmd = NULL;
	get_max_len(0, REINIT);
	if ((last_word = get_last_word(vct)) == NULL)
		return (0);
	len = ft_strlen(last_word);
	while (i < NB_CMD)
	{
		if (len == 0 || ft_strnequ(last_word, get_keyword(i), len) == true)
		{
			node = ft_lstnew(get_keyword(i), ft_strlen(get_keyword(i)) + 1);
			if (node != NULL)
				ft_lstadd_back(&possible_cmd, node);
			get_max_len(ft_strlen(get_keyword(i)), SET);
		}
		i++;
	}
	ft_strdel(&last_word);
	return (process_completion(vct, possible_cmd, len));
}
