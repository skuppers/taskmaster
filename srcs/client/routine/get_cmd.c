/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 11:32:32 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/11 17:47:39 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static enum e_cmd_type	get_cmd_type(t_vector *word)
{
	uint8_t	i;

	i = 0;
	while (i < NB_CMD)
	{
		if (ft_strequ(vct_getstr(word), get_keyword(i)) == true)
			return ((enum e_cmd_type)i);
		i++;
	}
	return (BAD_CMD);
}

static t_vector			*clean_line(t_vector *line)
{
	vct_replacechar(line, '\n', ' ');
	vct_replacechar(line, '\t', ' ');
	vct_trimfront(line, " ");
	return (line);
}

t_vector				*get_status(const uint8_t flag)
{
	t_vector	*line;
	t_vector	*feedback;

	line = vct_newstr(flag == KEEP_FEEDBACK ? "status [[progs]]" : "status");
	feedback = routine(line, flag);
	vct_del(&line);
	return (feedback);
}

t_cmd					*get_cmd(t_vector *line)
{
	t_cmd				*cmd;
	t_vector			*cmd_string;
	enum e_cmd_type		cmd_type;

	cmd = NULL;
	cmd_string = vct_splitchr(clean_line(line), ' ', DEL_CHAR);
	cmd_type = get_cmd_type(cmd_string);
	if (cmd_type == BAD_CMD)
		ft_dprintf(STDERR_FILENO, "*** Unknown syntax: %s %s\n",
					vct_getstr(cmd_string), vct_getstr(line));
	else
	{
		vct_trimfront(line, " ");
		cmd = get_cmd_struct(cmd_type, line);
	}
	vct_del(&cmd_string);
	return (cmd);
}
