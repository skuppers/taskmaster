/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 17:59:53 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 18:41:32 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"

void	del_hist(void *mem, size_t size)
{
	(void)size;
	ft_strdel(&((t_hist *)mem)->cmd);
}

char	*history(t_vector *line, uint8_t flag)
{
	static	t_list	*cur_hist = NULL;
	static	t_list	*queue_hist = NULL;
	static	t_list	*head_hist = NULL;
	t_list			*hist_node;
	t_hist			hist;

	if (flag == ADD)
	{
		hist.cmd = vct_dupstr(line);
		hist.prev = NULL;
		if (queue_hist != NULL)
			hist.prev = queue_hist;
		hist_node = ft_lstnew(&hist, sizeof(t_hist));
		ft_lstadd_back(&head_hist, hist_node);
		queue_hist = hist_node;
		cur_hist = queue_hist;
	}
	else if (flag == NEXT)
	{
		if (cur_hist != NULL && cur_hist->next != NULL)
			cur_hist = cur_hist->next;
		if (cur_hist != NULL)
			return (((t_hist *)(cur_hist->content))->cmd);
		return (NULL);
	}
	else if (flag == PREV)
	{
		if (cur_hist != NULL && ((t_hist *)(cur_hist->content))->prev != NULL)
			cur_hist = ((t_hist *)(cur_hist->content))->prev;
		if (cur_hist != NULL)
			return (((t_hist *)(cur_hist->content))->cmd);
		return (NULL);
	}
	else if (flag == FLUSH)
		ft_lstdel(&head_hist, del_hist);
	return (NULL);
}
