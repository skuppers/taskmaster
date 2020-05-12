/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 20:20:57 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 02:44:32 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

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

t_list			*cmd_comp(char *last_word)
{
	t_list		*possible_cmd;
	uint8_t		i;
	size_t		len;

	len = ft_strlen(last_word);
	possible_cmd = NULL;
	i = 0;
	while (i < NB_CMD)
	{
		if (len == 0 || ft_strnequ(last_word, get_keyword(i), len) == true)
			lst_add_node(&possible_cmd, get_keyword(i));
		i++;
	}
	return (possible_cmd);
}

t_list			*no_comp(char *last_word)
{
	(void)last_word;
	return (NULL);
}

int8_t			completion(t_vector *vct)
{
	t_list				*possible_cmd;
	static t_comp_type	comp_type[] = {no_comp, cmd_comp,
										instance_comp, prog_comp};
	char				*last_word;
	size_t				len;

	get_max_len(0, REINIT);
	if ((last_word = get_last_word(vct)) == NULL)
		return (0);
	len = ft_strlen(last_word);
	possible_cmd = comp_type[get_type_of_completion(vct)](last_word);
	ft_strdel(&last_word);
	return (process_completion(vct, possible_cmd, len));
}
