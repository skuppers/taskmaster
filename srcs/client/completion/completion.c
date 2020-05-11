/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 20:20:57 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/11 18:59:43 by ffoissey         ###   ########.fr       */
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
	while (i >= 0 && vct_getcharat(vct, i) != ' '
			&& vct_getcharat(vct, i) != ';')
		i--;
	if (ft_strclen(vct_getstr(vct), ' ') < ft_strclen(vct_getstr(vct), ';'))
		return (ft_strcdup(vct_getstr(vct) + i + 1, ' '));
	return (ft_strcdup(vct_getstr(vct) + i + 1, ';'));
}

size_t			get_max_len(const size_t len, const uint8_t flag)
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

enum e_comp_type		get_comp(t_vector *cmd)
{
	uint8_t				i;
	char				*str;
	enum e_comp_type	type;

	i = 0;
	if (vct_len(cmd) == 0)
		return (CMD_COMP);
	str = vct_dupstr(cmd);
	if (vct_clen(cmd, ' ') != vct_len(cmd))
		vct_cutfrom(cmd, vct_clen(cmd, ' '));
	type = CMD_COMP;
	while (i < NB_CMD)
	{
		if (ft_strequ(vct_getstr(cmd), get_keyword(i)) == true
			&& vct_len(cmd) != ft_strlen(str))
		{
			if (i == ADD || i == REMOVE || i == UPDATE)
				type = PROG_COMP;
			else if (i == FG || i == RESTART || i == PID || i == SIGNAL
				|| i == START || i == STATUS || i == STOP || i == TAIL
				|| i == CLEAR)
				type = INSTANCE_COMP;
			else if (i == HELP)
				type = CMD_COMP;
			else
				type = NO_COMP;
			break ;
		}
		i++;
	}
	ft_strdel(&str);
	return (type);
}

enum e_comp_type		get_type_of_completion(t_vector	*vct)
{
	t_vector			*cpy;
	enum e_comp_type	type_of_completion;	
	ssize_t				cur_cmd_index;

	cpy = vct_dup(vct);
	vct_trimfront(cpy, " \t");
	cur_cmd_index = vct_chr(vct, ';');
	if (cur_cmd_index != FAILURE && (size_t)cur_cmd_index != vct_len(cpy))
	{
		vct_popfrom(cpy, (size_t)cur_cmd_index + 1);
		vct_trimfront(cpy, " \t");
	}
	type_of_completion = get_comp(cpy);
//	ft_dprintf(2, "\n\ncomp = %d", type_of_completion);
	vct_del(&cpy);
	return (type_of_completion);
}

void			lst_add_node(t_list **list, const char *str)
{
	t_list	*node;
	
	node = ft_lstnew(str, ft_strlen(str) + 1);
	if (node != NULL)
		ft_lstadd_back(list, node);
	get_max_len(ft_strlen(str), SET);
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

void			fill_prog_name(t_list **list, char *last_word, t_vector *vct)
{
	if (ft_strlen(last_word) == 0 || ft_strnequ(last_word, vct_getstr(vct),
						ft_strlen(last_word)) == true)
		lst_add_node(list, vct_getstr(vct));
}

void			fill_instance_name(t_list **list, char *last_word, t_vector *vct)
{
	char	**tab;
	uint8_t	i;

	tab = ft_strsplit(vct_getstr(vct), ';');
	i = 0;
	if (tab != NULL)
	{
		while (tab[i] != NULL)
		{
			if (ft_strlen(last_word) == 0 || ft_strnequ(last_word, tab[i],
							ft_strlen(last_word)) == true)
				lst_add_node(list, tab[i]);
			i++;
		}
	}
	ft_free_tab_str(tab);
}

t_list			*get_prog_list(char *last_word, uint8_t mod, t_fill_list fill)
{
	uint8_t		i;
	t_list		*list;
	t_vector	*feedback;
	t_vector	*split;

	list = NULL;
	feedback = get_status(KEEP_FEEDBACK);
	if (feedback == NULL)
		return (NULL);
	i = 0;
	while ((split = vct_split(feedback, "/", NO_SEP)) != NULL)
	{
		if (i % 2 == mod)
			fill(&list, last_word, split);
		vct_del(&split);
		i++;
	}
	vct_del(&feedback);
	return (list);
}

t_list			*prog_comp(char *last_word)
{
	return (get_prog_list(last_word, 0, fill_prog_name));
}

t_list			*instance_comp(char *last_word)
{
	return (get_prog_list(last_word, 1, fill_instance_name));
}

int8_t			completion(t_vector *vct)
{
	t_list				*possible_cmd;
	static t_comp_type	comp_type[] = {no_comp, cmd_comp, instance_comp, prog_comp};
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
