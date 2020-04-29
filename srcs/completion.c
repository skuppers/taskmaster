/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 20:20:57 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 21:21:35 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"

static char	*get_last_word(t_vector *vct)
{
	int		i;

	i = vct_len(vct) == 0 ? 0 : vct_len(vct) - 1;
	if (i == 0)
		return (ft_strdup(vct_getstr(vct)));
	while (i >= 0 && vct_getcharat(vct, i) != ' ')
		i--;
	return (ft_strcdup(vct_getstr(vct) + i + 1, ' '));
}

void	print_completion(t_list *lst)
{
	if (lst != NULL)
	{
		print_completion(lst->next);
		ft_dprintf(STDERR_FILENO, "[%s]\n", (char *)(lst->content));
		free(lst->content);
		free(lst);
	}
}

void	completion(t_vector *vct)
{
	char		*last_word;
	size_t		len;
	uint8_t		i;
	t_list		*possible_cmd;
	t_list		*node;

	possible_cmd = NULL;
	last_word = get_last_word(vct); // MALLOC PROTECT
	len = ft_strlen(last_word);
	while (i < NB_CMD)
	{
		if (len == 0 || ft_strnequ(last_word, get_keyword(i), len) == TRUE)
		{
			node = ft_lstnew(get_keyword(i), ft_strlen(get_keyword(i)) + 1);
			ft_lstadd(&possible_cmd, node);
		}
		i++;
	}
	if (possible_cmd != NULL && possible_cmd->next == NULL)
	{
		vct_cutfrom(vct, vct_len(vct) - len);
		vct_addstr(vct, (char *)(possible_cmd->content));
		vct_add(vct, ' ');
		free(possible_cmd->content);
		free(possible_cmd);
	}
	else
	{
		ft_putendl_fd("\n", STDERR_FILENO);
		print_completion(possible_cmd);
	}
	ft_strdel(&last_word);
}
