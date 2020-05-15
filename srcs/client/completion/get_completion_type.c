/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_completion_type.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/11 19:05:41 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/15 22:11:41 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static enum e_comp_type		get_type(const uint8_t i)
{
	enum e_comp_type type;

	type = NO_COMP;
	if (i == ADD || i == REMOVE || i == UPDATE)
		type = PROG_COMP;
	else if (i == FG || i == RESTART || i == PID || i == SIGNAL
				|| i == START || i == STATUS || i == STOP || i == TAIL
				|| i == CLEAR)
		type = INSTANCE_COMP;
	else if (i == HELP)
		type = CMD_COMP;
	return (type);
}

static enum e_comp_type		get_comp(t_vector *cmd)
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
			type = get_type(i);
			break ;
		}
		i++;
	}
	ft_strdel(&str);
	return (type);
}

enum e_comp_type			get_type_of_completion(t_vector *vct)
{
	t_vector			*cpy;
	enum e_comp_type	type_of_completion;
	ssize_t				cur_cmd_index;
	const char			*last;

	cpy = vct_dup(vct);
	vct_trimfront(cpy, " \t");
	cur_cmd_index = FAILURE;
	if ((last = ft_strrchr(vct_getstr(vct), ';')) != NULL)
		cur_cmd_index = last - vct_getstr(vct);
	if (cur_cmd_index != FAILURE && (size_t)cur_cmd_index != vct_len(cpy))
	{
		vct_popfrom(cpy, (size_t)cur_cmd_index + 1);
		vct_trimfront(cpy, " \t");
	}
	type_of_completion = get_comp(cpy);
	vct_del(&cpy);
	return (type_of_completion);
}
