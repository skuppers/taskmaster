/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 17:19:43 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 17:47:06 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void			free_env(void *node, size_t content_size)
{
	free_node(node, content_size);
	free(node);
}

void			del_prgm(void *node, size_t content_size)
{
	(void)content_size;
	if (node != NULL)
		ft_strdel(&((t_program *)node)->name);
	free(node);
}

void			del_group(void *node, size_t content_size)
{
	(void)content_size;
	if (node != NULL)
	{
		ft_lstdel(&((t_group *)node)->prog_list, NULL);
		free(((t_group *)node)->name);
		free(node);
	}
}

void free_inifile(dictionary *dict)
{
	iniparser_freedict(dict);
}
