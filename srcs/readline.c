/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_vct_readline.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/22 17:22:54 by ffoissey         ###   ########.fr       */
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
            env->actionkeys[index](env, vct);
		++index;
	}
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
                ft_putstr(buf);
                if (vct_addnstr(rest, buf, (size_t)ret) == FAILURE)
				    return (FAILURE);
            }
			else
                handle_actionkey(env, buf, vct);

			ft_bzero(buf, BUFF_SIZE);
			
			if (vct_chr(rest, '\n') != FAILURE)
				break ;
		}
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
