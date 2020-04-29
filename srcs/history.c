/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 17:59:53 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 19:40:41 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"

static void	del_hist(void *mem, size_t size)
{
	(void)size;
	ft_strdel(&((t_hist *)mem)->cmd);
}

static char	*get_next_entry(t_list **cur)
{
	if (*cur != NULL && (*cur)->next != NULL)
		*cur = (*cur)->next;
	if (*cur != NULL)
		return (((t_hist *)((*cur)->content))->cmd);
	return (NULL);
}

static char	*get_prev_entry(t_list *queue, t_list **cur, const uint8_t flag)
{
	static uint8_t	end = TRUE;

	if (flag == RESET)
	{
		end = TRUE;
		return (NULL);
	}
	if (*cur != NULL && ((t_hist *)((*cur)->content))->prev != NULL)
	{
		if (*cur == queue && end == TRUE)
		{
			end = FALSE;
			return (((t_hist *)((*cur)->content))->cmd);
		}
		*cur = ((t_hist *)((*cur)->content))->prev;
	}
	if (*cur != NULL)
		return (((t_hist *)((*cur)->content))->cmd);
	return (NULL);
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

char		*history(t_vector *line, uint8_t flag)
{
	static	t_list	*cur = NULL;
	static	t_list	*queue = NULL;
	static	t_list	*head = NULL;

	if (flag & ADD)
		cur = add_entry(&queue, &head, line);
	else if (flag & NEXT)
		return (get_next_entry(&cur));
	else if (flag & PREV)
		return (get_prev_entry(queue, &cur, NOFLAG));
	else if (flag & FLUSH)
		ft_lstdel(&head, del_hist);
	else if (flag & RESET)
	{
		cur = queue;
		get_prev_entry(NULL, NULL, RESET);
	}
	return (NULL);
}
