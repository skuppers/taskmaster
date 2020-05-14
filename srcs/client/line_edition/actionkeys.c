/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actionkeys.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 22:31:31 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

int8_t		ak_arrow_up(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	char	*cmd;

	(void)c;
	cmd = history(vct, PREV);
	if (cmd != NULL)
	{
		vct_clear(vct);
		vct_addstr(vct, cmd);
	}
	ak_home(env, vct, NULL);
	ft_putstr_fd("\33[J", STDIN_FILENO);
	vct_print_fd(vct, STDIN_FILENO);
	calc_after_totalprint(env, vct);
	return (0);
}

int8_t		ak_arrow_down(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	char	*cmd;

	(void)c;
	cmd = history(vct, NEXT);
	if (cmd != NULL && cmd != vct_getstr(vct))
	{
		vct_clear(vct);
		vct_addstr(vct, cmd);
	}
	ak_home(env, vct, NULL);
	ft_putstr_fd("\33[J", STDIN_FILENO);
	vct_print_fd(vct, STDIN_FILENO);
	calc_after_totalprint(env, vct);
	return (0);
}

int8_t		ak_arrow_left(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)vct;
	if (env->cursoridx > 0)
	{
		ft_putstr_fd(c, STDIN_FILENO);
		dec_x(env, 1);
		env->cursoridx--;
	}
	return (0);
}

int8_t		ak_arrow_right(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	if (env->cursoridx < vct_len(vct))
	{
		ft_putstr_fd(c, STDIN_FILENO);
		inc_x(env, 1);
		env->cursoridx++;
	}
	return (0);
}
