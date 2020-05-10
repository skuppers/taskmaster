/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addchar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 23:04:27 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static void		put_in_middle(t_env *env, t_vector *dest, char *src)
{
	uint32_t	tmpidx;

	vct_addcharat(dest, src[0], env->cursoridx);
	tmpidx = env->cursoridx;
	ak_home(env, dest, NULL);
	vct_print_fd(dest, STDIN_FILENO);
	calc_after_totalprint(env, dest);
	ak_home(env, dest, NULL);
	while (tmpidx-- > 0)
		ak_arrow_right(env, dest, "\33[C");
	ak_arrow_right(env, dest, "\33[C");
}

static void		put_first(t_env *env, t_vector *dest, char *src)
{
	vct_pushstr(dest, src);
	vct_print_fd(dest, STDIN_FILENO);
	calc_after_totalprint(env, dest);
	ak_home(env, dest, NULL);
	ak_arrow_right(env, dest, "\33[C");
}

int8_t			putchar_in_vct(t_env *env, t_vector *dest,
					char *src, size_t size)
{
	history(NULL, RESET);
	if (ft_strequ(src, "\n") == 1)
	{
		ft_putchar_fd('\n', STDIN_FILENO);
		if (vct_addnstr(dest, src, size) == FAILURE)
			return (FAILURE);
	}
	else if (env->cursoridx == vct_len(dest))
	{
		if (vct_addnstr(dest, src, size) == FAILURE)
			return (FAILURE);
		ft_putstr_fd(src, STDIN_FILENO);
		env->cursoridx += ft_strlen(src);
		pinc_x(env);
	}
	else if (env->cursoridx == 0)
		put_first(env, dest, src);
	else
		put_in_middle(env, dest, src);
	return (0);
}
