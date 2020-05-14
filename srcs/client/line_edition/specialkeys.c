/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specialkeys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 22:31:31 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

int8_t			ak_home(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	(void)vct;
	while (env->cursoridx > 0)
	{
		ak_arrow_left(env, vct, "\33[D");
	}
	return (0);
}

int8_t			ak_end(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	while (env->cursoridx < vct_len(vct))
		ak_arrow_right(env, vct, "\33[C");
	return (0);
}

int8_t			ak_delete(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	uint32_t	tmpidx;

	(void)c;
	if (env->cursoridx == vct_len(vct) - 1)
	{
		vct_cut(vct);
		ft_putstr_fd("\33[K", STDERR_FILENO);
		dec_x(env, 1);
	}
	else if (env->cursoridx < vct_len(vct) - 1)
	{
		vct_delchar(vct, env->cursoridx);
		tmpidx = env->cursoridx;
		ak_home(env, vct, NULL);
		ft_putstr_fd("\33[J", STDIN_FILENO);
		vct_print_fd(vct, STDIN_FILENO);
		calc_after_totalprint(env, vct);
		ak_home(env, vct, NULL);
		while (tmpidx-- > 0)
			ak_arrow_right(env, vct, "\33[C");
	}
	return (0);
}

int8_t			ak_backspace(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	uint32_t	tmpidx;

	(void)c;
	if (env->cursoridx == vct_len(vct) && env->cursoridx > 0)
	{
		vct_cut(vct);
		ak_arrow_left(env, vct, "\33[D");
		ft_putstr_fd("\33[K", STDIN_FILENO);
	}
	else if (env->cursoridx > 0)
	{
		vct_delchar(vct, env->cursoridx - 1);
		tmpidx = env->cursoridx;
		ak_home(env, vct, NULL);
		ft_putstr_fd("\33[J", STDIN_FILENO);
		vct_print_fd(vct, STDIN_FILENO);
		calc_after_totalprint(env, vct);
		ak_home(env, vct, NULL);
		while (tmpidx-- > 1)
			ak_arrow_right(env, vct, "\33[C");
	}
	return (0);
}

int8_t			ak_hightab(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)env;
	(void)c;
	if (completion(vct) == 0)
	{
		ak_home(env, vct, NULL);
		ft_putstr_fd("\33[J", STDIN_FILENO);
		vct_print_fd(vct, STDIN_FILENO);
		calc_after_totalprint(env, vct);
	}
	else
	{
		print_prompt(env);
		vct_print_fd(vct, STDIN_FILENO);
		calc_after_totalprint(env, vct);
	}
	return (0);
}
