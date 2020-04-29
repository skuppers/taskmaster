/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actionkeys.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 18:19:39 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"

int8_t		ak_arrow_up(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	char	*cmd;

	cmd = history(vct, PREV);
	if (cmd != NULL)
	{
		vct_clear(vct);
		vct_addstr(vct, cmd);
	}
	while (env->cursoridx-- > 0)
		ft_putstr("\33[D");
	vct_print(vct);
	env->cursoridx = vct_len(vct);
	return (0);
}

int8_t		ak_arrow_down(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	char	*cmd;

	cmd = history(vct, NEXT);
	if (cmd != NULL)
	{
		vct_clear(vct);
		vct_addstr(vct, cmd);
	}
	 while (env->cursoridx-- > 0)
		ft_putstr("\33[D");
	vct_print(vct);
	env->cursoridx = vct_len(vct);
	return (0);
}


int8_t		ak_arrow_left(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)vct;
	if (env->cursoridx > 0)
	{
		ft_putstr(c);
		env->cursoridx--;
	}
	return (0);
}

int8_t		ak_arrow_right(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	if (env->cursoridx < vct_len(vct))
	{
		ft_putstr(c);
		env->cursoridx++;
	}
	return (0);
}

int8_t		ak_home(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;(void)vct;
	while (env->cursoridx > 0)
	{
		ft_putstr("\33[D");
		env->cursoridx--;
	}
	return (0);
}

int8_t		ak_end(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	while (env->cursoridx < vct_len(vct))
	{
		ft_putstr("\33[C");
		env->cursoridx++;
	}
	return (0);
}

int8_t		ak_delete(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	uint32_t     tmpidx;

	if (env->cursoridx == vct_len(vct) - 1)
	{
		vct_cut(vct);
		ft_putstr("\33[K");
	}
	else if (env->cursoridx < vct_len(vct) - 1)
	{
		vct_delchar(vct, env->cursoridx);
		tmpidx = env->cursoridx;
		while (tmpidx-- > 0)
			ft_putstr("\33[D");
		ft_putstr("\33[K");
		vct_print(vct);
		tmpidx = vct_len(vct);
		while (tmpidx-- > 0)
			ft_putstr("\33[D");
		while (tmpidx++ != env->cursoridx - 1)
			ft_putstr("\33[C");
	}
	return (0);
}

int8_t		ak_backspace(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	uint32_t     tmpidx;

	if (env->cursoridx == vct_len(vct) && env->cursoridx > 0)
	{
		vct_cut(vct);
		ft_putstr("\33[D");
		ft_putstr("\33[K");
		env->cursoridx--;
	}
	else if (env->cursoridx > 0)
	{
		vct_delchar(vct, env->cursoridx - 1);
		tmpidx = env->cursoridx;
		while (tmpidx-- > 0)
			ft_putstr("\33[D");
		ft_putstr("\33[K");
		vct_print(vct);
		tmpidx = vct_len(vct);
		while (tmpidx-- > 0)
			ft_putstr("\33[D");
		while (tmpidx++ != env->cursoridx - 2)
			ft_putstr("\33[C");
		env->cursoridx = env->cursoridx - 1;
	}
	return (0);
}

int8_t		ak_ctrl_d(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)env;(void)c;
	if (vct_len(vct) == 0)
	{
		ft_putstr("ciao\n");
		exit_routine();
	}
	return (0);
}

int8_t		ak_ctrl_l(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	uint32_t     tmpidx;

	ft_putstr("\33[2J");
	ft_putstr("\33[0;0f");
	ft_putstr("taskmaster> ");
	vct_print(vct);
	tmpidx = vct_len(vct);
	while (tmpidx-- > 0)
		ft_putstr("\33[D");
	while (tmpidx++ != env->cursoridx - 1)
			ft_putstr("\33[C");
	return (0);
}

int8_t		ak_ctrl_r(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)env;(void)vct;(void)c;
	return (0);
}
int8_t		ak_hightab(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)env;(void)vct;(void)c;
	return (0);
}
