/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 18:44:25 by ffoissey         ###   ########.fr       */
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
		ft_putchar('\n');
		if (vct_addnstr(dest, src, size) == FAILURE)
			return (FAILURE);
	}
	return (0);
}

static int8_t   putchar_in_vct(t_env *env, t_vector *dest, char *src, size_t size)
{
	uint32_t	tmpidx;

	if (put_newline(dest, src, size) != 0)
		return (-1);

	if (env->cursoridx == vct_len(dest)) // are we at the end ? matches very first character too
	{
		if (vct_addnstr(dest, src, size) == FAILURE)
			return (FAILURE);
		ft_putstr(src);
		env->cursoridx += ft_strlen(src);
	}
	else if (env->cursoridx == 0)
	{
		vct_pushstr(dest, src);
		tmpidx = vct_len(dest);
		vct_print(dest);
		while (tmpidx-- > 1)
			ft_putstr("\33[D");
		env->cursoridx = 1;
	}
	else
	{
		vct_addcharat(dest, src[0], env->cursoridx);
		tmpidx = env->cursoridx;
		while (env->cursoridx-- > 0)
		{
			ft_putstr("\33[D");
		}
		vct_print(dest);
		env->cursoridx = vct_len(dest);	
		while (env->cursoridx != tmpidx + 1)
		{
			ft_putstr("\33[D");
			env->cursoridx--;
		}
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

void		handle_resize(t_env *env)
{
	struct winsize w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    env->winhei = w.ws_row;
    env->winwid = w.ws_col;
	// TODO: handle resize
//	write(1, "\33[6n", 4);
}

int8_t		handle_signal(t_env *env)
{
	if (env->sigint == 1)
	{
		env->sigint = 0;
		ft_dprintf(STDERR_FILENO, "\n");
	}
	else if (env->sigwinch == 1)
	{
		env->sigwinch = 0;
		handle_resize(env);
	}
	return (0);
}

static int	read_next(t_vector *vct, t_vector *rest, const int fd, t_env *env)
{
	char		buf[BUFF_SIZE];
	t_vector	*line;
	ssize_t		ret;

	ret = LINE_OK;
	env->cursoridx = 0;
	if (vct_chr(rest, '\n') == FAILURE)
	{
		env->cursoridx = 0;
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

		if (signal_catched(env))
			return (handle_signal(env));

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
			rest = vct_new(DFL_VCT_SIZE);
		if (rest != NULL)
		{
			ret = read_next(vct, rest, fd, env);
			if (ret <= 0)
				vct_del(&rest);
		}
	}
	return (ret);
}
