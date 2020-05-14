/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 23:04:27 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void		dec_x(t_env *env, int goup)
{
	int		i;

	i = env->winwid - 1;
	if (env->cursorx == 0)
	{
		env->cursorx = env->winwid - 1;
		env->cursory -= 1;
		if (goup == 1)
		{
			ft_putstr_fd("\33[A", STDIN_FILENO);
			while (i-- > 0)
				ft_putstr_fd("\33[C", STDIN_FILENO);
		}
	}
	else
	{
		env->cursorx -= 1;
	}
}

void		inc_x(t_env *env, int godown)
{
	int		i;

	i = env->winwid - 1;
	if (env->cursorx >= env->winwid - 1)
	{
		env->cursorx = 0;
		env->cursory += 1;
		if (godown == 1)
		{
			ft_putstr_fd("\33[B", STDIN_FILENO);
			while (i-- > 0)
				ft_putstr_fd("\33[D", STDIN_FILENO);
		}
	}
	else
	{
		env->cursorx += 1;
	}
}

void		pinc_x(t_env *env)
{
	int		i;

	i = env->winwid - 1;
	if (env->cursorx >= env->winwid - 1)
	{
		env->cursorx = 0;
		env->cursory += 1;
		ft_putstr_fd("\33[B", STDIN_FILENO);
		while (i-- > 0)
			ft_putstr_fd("\33[D", STDIN_FILENO);
	}
	else
	{
		env->cursorx += 1;
	}
}

void		calc_after_totalprint(t_env *env, t_vector *vct)
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
			ft_putstr_fd("\33[B", STDIN_FILENO);
			while (i-- > 0)
				ft_putstr_fd("\33[D", STDIN_FILENO);
		}
	}
	env->cursoridx = vct_len(vct);
}
