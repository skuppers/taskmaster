/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 22:16:04 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"

uint64_t			compute_mask(char c[BUFF_SIZE])
{
	uint16_t	shift;
	uint16_t	index;
	uint64_t	value;
	uint64_t	tmp;

	shift = 56;
	index = 0;
	value = 0;
	while (index < 8 && c[index] != '\0')
	{
		tmp = c[index++];
		tmp <<= shift;
		value |= tmp;
		shift -= 8;
	}
	return (value);
}

static void			handle_actionkey(t_env *env, char c[BUFF_SIZE], t_vector *vct)
{
	uint32_t	index;
	uint64_t	value;

	index = 0;
	value = compute_mask(c);
	while (index < AK_AMOUNT)
	{
		if (value == env->ak_masks[index])
            env->actionkeys[index](env, vct, c);
		++index;
	}
}

int8_t				put_newline(t_vector *dest, char *src, size_t size)
{
	if (ft_strequ(src, "\n") == 1)
	{
		ft_putchar_fd('\n', STDERR_FILENO);
		if (vct_addnstr(dest, src, size) == FAILURE)
			return (FAILURE);
	}
	return (0);
}



void		dec_x(t_env *env, int goup)
{
	int i;

	i = env->winwid - 1;
	if (env->cursorx == 0)
	{
		env->cursorx = env->winwid - 1;
		env->cursory -= 1;
		if (goup == 1)
		{
			ft_putstr_fd("\33[A", STDERR_FILENO);
			while (i-- > 0)
				ft_putstr_fd("\33[C", STDERR_FILENO);
		}
	}
	else
	{
		env->cursorx -= 1;
	}
}

void		inc_x(t_env *env, int godown)
{
	int i;

	i = env->winwid - 1;
	if (env->cursorx >= env->winwid - 1)
	{
		env->cursorx = 0;
		env->cursory += 1;
		if (godown == 1)
		{
			ft_putstr_fd("\33[B", STDERR_FILENO);
			while (i-- > 0)
				ft_putstr_fd("\33[D", STDERR_FILENO);
		}
	}
	else
	{
		env->cursorx += 1;
	}
}
void		pinc_x(t_env *env)
{
	int i;

	i = env->winwid - 1;
	if (env->cursorx >= env->winwid - 1)
	{
		env->cursorx = 0;
		env->cursory += 1;
		ft_putstr_fd("\33[B", STDERR_FILENO);
		while (i-- > 0)
			ft_putstr_fd("\33[D", STDERR_FILENO);
	}
	else
	{
		env->cursorx += 1;
	}
}
void	calc_after_totalprint(t_env *env, t_vector *vct)
{
	size_t	len;
	int		i;

	len = vct_len(vct);
	len += ft_strlen(env->opt.str[PROMPT]);
	if (len < env->winwid)
	{
		env->cursorx = len;
	}
	else
	{
		env->cursory = (len / env->winwid);
		env->cursorx = (len % env->winwid);
		if (env->cursorx == 0)
		{
			i = env->winwid - 1;
			ft_putstr_fd("\33[B", STDERR_FILENO);
			while (i-- > 0)
				ft_putstr_fd("\33[D", STDERR_FILENO);
		}
	}
	env->cursoridx = vct_len(vct);
}

static int8_t   putchar_in_vct(t_env *env, t_vector *dest, char *src, size_t size)
{
	uint32_t	tmpidx;

	if (ft_strequ(src, "\n") == 1)
	{
		ft_putchar_fd('\n', STDERR_FILENO);
		if (vct_addnstr(dest, src, size) == FAILURE)
			return (FAILURE);
	}
	else if (env->cursoridx == vct_len(dest))
	{
		if (vct_addnstr(dest, src, size) == FAILURE)
			return (FAILURE);
		ft_putstr_fd(src, STDERR_FILENO);
		env->cursoridx += ft_strlen(src);
		pinc_x(env);
	}
	else if (env->cursoridx == 0)
	{
		vct_pushstr(dest, src);
		vct_print_fd(dest, STDERR_FILENO);
		calc_after_totalprint(env, dest);
		ak_home(env, dest, NULL);
		ak_arrow_right(env, dest, "\33[C");
	}
	else
	{
		vct_addcharat(dest, src[0], env->cursoridx);
		tmpidx = env->cursoridx;
		ak_home(env, dest, NULL);
		vct_print_fd(dest, STDERR_FILENO);
		calc_after_totalprint(env, dest);
		ak_home(env, dest, NULL);
		while (tmpidx-- > 0)
		{
			ak_arrow_right(env, dest, "\33[C");
		}
		ak_arrow_right(env, dest, "\33[C");
	}
	return (0);
}

int8_t		signal_catched(t_env *env)
{
	
	if (env->sigint == 1)
	{
		return (1);
	}
	else if (env->sigwinch == 1)
	{
		return (1);
	}
	return (0);
}

int	print_prompt(t_env *env)
{
	env->cursorx = ft_strlen(env->opt.str[PROMPT]);
	if (env->winwid > 0)
		env->cursory = ft_strlen(env->opt.str[PROMPT]) / env->winwid;
	return (ft_dprintf(STDERR_FILENO, "%s", env->opt.str[PROMPT]));
}

int8_t		handle_signal(t_env *env, t_vector *vct)
{
	if (env->sigint == 1)
	{
		env->sigint = 0;
		ak_end(env, vct, NULL);
		ft_dprintf(STDERR_FILENO, "\n");
		return (0);
	}
	else if (env->sigwinch == 1)
	{
		env->sigwinch = 0;
		env->szchanged = 1;
		update_winsize(env);
		ak_ctrl_l(env, vct, NULL);
		return (1);
	}
	return (0);
}

static int	read_next(t_vector *vct, t_vector *rest, const int fd, t_env *env)
{
	char		buf[BUFF_SIZE];
	t_vector	*line;
	ssize_t		ret;

	ret = LINE_OK;
	if (vct_chr(rest, '\n') == FAILURE)
	{
		ft_bzero(buf, BUFF_SIZE);
		while ((ret = read(fd, buf, BUFF_SIZE)) > 0)
		{
			if (ft_strcheck(buf, ft_isprint) || buf[0] == '\n')
			{
                if (putchar_in_vct(env, rest, buf, (size_t)ret) == FAILURE)
                    return (FAILURE);
            }
			else
                handle_actionkey(env, buf, rest);

			ft_bzero(buf, BUFF_SIZE);
			if (vct_chr(rest, '\n') != FAILURE)
				break ;
		}

		if (signal_catched(env) != 0)
			return (handle_signal(env, rest));

		if (ret == FAILURE)
			return (FAILURE);
	}
	line = vct_splitchr(rest, '\n', DEL_CHAR);
	if (vct_cpy(vct, line) == FAILURE)
		ret = FAILURE;
	if (ret == IS_EOF && vct_len(vct) > 0)
		ret = LINE_OK;
	vct_del(&line);
	return ((int)ret);
}

int			tsk_readline(t_vector *vct, const int fd, t_env *env)
{
	static t_vector		*rest = NULL;
	int					ret;
	
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
	ret = FAILURE;
	if (fd == CLEANUP)
	{
		ret = SUCCESS;
		vct_del(&rest);
	}
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
			{
				vct_del(&rest);
			}
		}
	}
	return (ret);
}
