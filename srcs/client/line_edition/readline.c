/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 14:17:32 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

int						print_prompt(t_env *env)
{
	env->cursorx = ft_strlen(env->opt.str[PROMPT]);
	if (env->winwid > 0)
		env->cursory = ft_strlen(env->opt.str[PROMPT]) / env->winwid;
	return (ft_dprintf(STDIN_FILENO, "%s", env->opt.str[PROMPT]));
}

static void				split_line(ssize_t *ret, t_vector *rest, t_vector *vct)
{
	t_vector			*line;

	line = vct_splitchr(rest, '\n', DEL_CHAR);
	if (vct_cpy(vct, line) == FAILURE)
		*ret = FAILURE;
	if (*ret == IS_EOF && vct_len(vct) > 0)
		*ret = LINE_OK;
	vct_del(&line);
}

static int				read_next(t_vector *vct, t_vector *rest, const int fd,
								t_env *env)
{
	char				buf[BUFF_SIZE];
	ssize_t				ret;

	ret = LINE_OK;
	if (vct_chr(rest, '\n') == FAILURE)
	{
		ft_bzero(buf, BUFF_SIZE);
		while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
		{
			if (ft_strcheck(buf, ft_isprint) == FALSE && buf[0] != '\n')
				handle_actionkey(env, buf, rest);
			else if (putchar_in_vct(env, rest, buf, (size_t)ret) == FAILURE)
				return (FAILURE);
			ft_bzero(buf, BUFF_SIZE);
			if (vct_chr(rest, '\n') != FAILURE)
				break ;
		}
		if (signal_catched(env) != 0)
			return (handle_signal(env, rest));
		if (ret == FAILURE)
			return (FAILURE);
	}
	split_line(&ret, rest, vct);
	return ((int)ret);
}

static void				winsize_changed(t_env *env)
{
	if (env->szchanged == 0)
	{
		update_winsize(env);
		print_prompt(env);
		env->cursoridx = 0;
	}
	else
		env->szchanged = 0;
	if (env->szchanged == 1)
		env->szchanged = 0;
}

int						tsk_readline(t_vector *vct, const int fd, t_env *env)
{
	static t_vector		*rest = NULL;
	int					ret;

	winsize_changed(env);
	ret = FAILURE;
	if (fd >= 0 && vct != NULL)
	{
		vct_reset(vct, 0);
		if (rest == NULL)
		{
			rest = vct_new(DFL_VCT_SIZE);
			g_env->cur_line = rest;
		}
		if (rest != NULL)
		{
			ret = read_next(vct, rest, fd, env);
			if (ret <= 0)
				vct_del(&rest);
		}
	}
	return (ret);
}
