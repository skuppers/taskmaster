/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sigcatcher.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 23:04:27 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

int8_t		signal_catched(t_env *env)
{
	if (env->sigint == 1)
		return (1);
	else if (env->sigwinch == 1)
		return (1);
	return (0);
}

int8_t		handle_signal(t_env *env, t_vector *vct)
{
	if (env->sigint == 1)
	{
		env->sigint = 0;
		ak_end(env, vct, NULL);
		history(NULL, RESET);
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
