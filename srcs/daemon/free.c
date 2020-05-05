/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 17:19:43 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 21:22:04 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void			free_env(void *node, size_t content_size)
{
	free_node(node, content_size);
	free(node);
}

void	clear_instances(t_instance *in)
{
	t_instance *next;

	while (in != NULL)
	{
		next = in->next;
		free(in->name);
		free(in);
		in = next;
	}
}

void			del_prgm(void *node, size_t content_size)
{
	(void)content_size;
	if (node != NULL)
	{
		ft_lstdel(&((t_program *)node)->env, free_env);
		ft_strdel(&((t_program *)node)->name);
		if (((t_program *)node)->instance != NULL)
			clear_instances(((t_program *)node)->instance);
		ft_free_tab_str(((t_program *)node)->avs);
		ft_free_tab_str(((t_program *)node)->exitcodes	);
	}
	free(node);
}

void free_inifile(dictionary *dict)
{
	iniparser_freedict(dict);
}
