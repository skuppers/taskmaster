/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dlrkeys.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/11 15:57:34 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

int8_t			ak_ctrl_d(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	if (vct_len(vct) == 0)
		exit_routine(EXIT_EOF);
	else
		ak_delete(env, vct, NULL);
	return (0);
}

int8_t			ak_ctrl_l(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	uint32_t	tmpidx;

	(void)c;
	tmpidx = env->cursoridx;
	ft_putstr_fd("\33[2J", STDERR_FILENO);
	ft_putstr_fd("\33[0;0f", STDERR_FILENO);
	print_prompt(env);
	vct_print_fd(vct, STDERR_FILENO);
	calc_after_totalprint(env, vct);
	ak_home(env, vct, NULL);
	while (tmpidx--)
	{
		ft_putstr_fd("\33[C", STDERR_FILENO);
		inc_x(env, 1);
		env->cursoridx++;
	}
	return (0);
}

int8_t			ak_ctrl_r(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)env;
	(void)vct;
	(void)c;
	return (0);
}
