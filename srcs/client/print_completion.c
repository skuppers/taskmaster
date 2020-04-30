/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_completion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 15:22:32 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 15:36:41 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"
#include <sys/ioctl.h>

static void			post_process(t_list *lst)
{
	char	*data;
	char	*new_data;

	while (lst != NULL)
	{
		new_data = NULL;
		data = (char *)lst->content;
		new_data = ft_asprintf("%-*s", get_max_len(0, GET) + 1, data);
		ft_strdel(&data);
		lst->content = (void *)new_data;
		lst = lst->next;
	}
}

static size_t		get_elem_by_line(int col)
{
	size_t	elem_by_line;

	if (col <= 0 || get_max_len(0, GET) + 1 <= 0)
		return (1);
	elem_by_line = col / (get_max_len(0, GET) + 1);
	if (elem_by_line == 0)
		return (1);
	return (elem_by_line);
}

static size_t		get_elem_by_col(t_list *lst)
{
	size_t			elem_by_col;
	size_t			elem_by_line;
	size_t			nb_elem;
	struct winsize	w;
	int				col;

	nb_elem = ft_lstsize(lst);
	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &w) != FAILURE)
		col = w.ws_col;
	else
		col = 0;
	elem_by_line = get_elem_by_line(col);
	elem_by_col = nb_elem / elem_by_line;
	if (nb_elem % elem_by_line)
		elem_by_col++;
	return (elem_by_col);
}

static void			process_print(t_list *lst, size_t elem_by_col)
{
	size_t			i;
	size_t			j;
	t_list			*tmp;

	i = 0;
	ft_putchar_fd('\n', 2);
	while (i < elem_by_col)
	{
		tmp = lst;
		j = i;
		while (j && tmp)
		{
			tmp = tmp->next;
			j--;
		}
		while (tmp != NULL)
		{
			if (j++ % elem_by_col == 0)
				ft_putstr_fd((char *)tmp->content, 2);
			tmp = tmp->next;
		}
		i++;
		if (i != elem_by_col)
			ft_putchar_fd('\n', 2);
	}
}

void				del_completion_list(void *mem, size_t content_size)
{
	(void)content_size;
	ft_memdel(&mem);
}

int8_t				print_completion(t_list *list)
{
	if (list != NULL)
	{ 
		post_process(list);
		process_print(list, get_elem_by_col(list));
		ft_putchar_fd('\n', STDERR_FILENO);
		ft_lstdel(&list, del_completion_list);
		return (-1);
	}
	return (0);
}
