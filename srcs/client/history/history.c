/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 17:59:53 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 23:06:05 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static void		del_hist(void *mem, size_t size)
{
	(void)size;
	ft_strdel(&((t_hist *)mem)->cmd);
}

static char		*get_next_entry(t_list **cur, const uint8_t flag, char **keep)
{
	if (flag & RESET)
	{
		ft_strdel(keep);
		return (NULL);
	}
	if (*cur == NULL)
		return (NULL);	
	if ((*cur)->next == NULL)
		return (*keep);
	*cur = (*cur)->next;
	return (((t_hist *)((*cur)->content))->cmd);
}

static char		*get_prev_entry(t_list **cur, const uint8_t flag)
{
	static bool	end = true;

	if (flag & RESET)
	{
		end = true;
		return (NULL);
	}
	if (*cur == NULL)
		return (NULL);
	if ((*cur)->next == NULL)
	{
		if (end == true)
		{
			end = false;
			return (((t_hist *)((*cur)->content))->cmd);
		}
		end = true;
	}
	if (((t_hist *)((*cur)->content))->prev != NULL)
		*cur = ((t_hist *)((*cur)->content))->prev;
	return (((t_hist *)((*cur)->content))->cmd);
}

static t_list	*add_entry(t_list **queue, t_list **head, t_vector *line)
{
	t_list			*hist_node;
	t_hist			hist;

	hist.cmd = vct_dupstr(line);
	hist.prev = NULL;
	if (queue != NULL)
		hist.prev = *queue;
	hist_node = ft_lstnew(&hist, sizeof(t_hist));
	ft_lstadd_back(head, hist_node);
	*queue = hist_node;
	return (*queue);
}

char			*history(t_vector *line, const uint8_t flag)
{
	static	t_list	*cur = NULL;
	static	t_list	*queue = NULL;
	static	t_list	*head = NULL;
	static char		*keep = NULL;

	if ((flag & ADD) || (flag & RESET))
	{
		cur = (flag & ADD) ? add_entry(&queue, &head, line) : queue;
		get_prev_entry(NULL, RESET);
		get_next_entry(NULL, RESET, &keep);
	}
	else if (flag & NEXT)
		return (get_next_entry(&cur, NOFLAG, &keep));
	else if (flag & PREV)
	{
		if (keep == NULL)
			keep = vct_dupstr(line);
		return (get_prev_entry(&cur, NOFLAG));
	}
	else if (flag & FLUSH)
	{
		ft_lstdel(&head, del_hist);
		get_next_entry(NULL, RESET, &keep);
	}
	return (NULL);
}
