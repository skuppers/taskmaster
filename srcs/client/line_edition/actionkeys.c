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
	ak_home(env, vct, NULL);
	ft_putstr_fd("\33[J", STDERR_FILENO);
	vct_print_fd(vct, STDERR_FILENO);
	calc_after_totalprint(env, vct);
	return (0);
}

int8_t		ak_arrow_down(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	char	*cmd;

	cmd = history(vct, NEXT);
//	dprintf(2, "%p\n%p\n", vct_getstr(vct), cmd);
	if (cmd != NULL && cmd != vct_getstr(vct))
	{
		vct_clear(vct);
		vct_addstr(vct, cmd);
	}
	ak_home(env, vct, NULL);
	ft_putstr_fd("\33[J", STDERR_FILENO);
	vct_print_fd(vct, STDERR_FILENO);
	calc_after_totalprint(env, vct);
	return (0);
}


int8_t		ak_arrow_left(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)vct;
	if (env->cursoridx > 0)
	{
		ft_putstr_fd(c, STDERR_FILENO);
		dec_x(env, 1);
		env->cursoridx--;
	}
	return (0);
}

int8_t		ak_arrow_right(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	if (env->cursoridx < vct_len(vct))
	{
		ft_putstr_fd(c, STDERR_FILENO);
		inc_x(env, 1);
		env->cursoridx++;
	}
	return (0);
}

int8_t		ak_home(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;(void)vct;
	while (env->cursoridx > 0)
	{
		ak_arrow_left(env, vct, "\33[D");
	}
	return (0);
}

int8_t		ak_end(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	while (env->cursoridx < vct_len(vct))
	{
		ak_arrow_right(env, vct, "\33[C");
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
		ft_putstr_fd("\33[K", STDERR_FILENO);
		dec_x(env, 1);
	}
	else if (env->cursoridx < vct_len(vct) - 1)
	{
		vct_delchar(vct, env->cursoridx);
		tmpidx = env->cursoridx;
		ak_home(env, vct, NULL);
		ft_putstr_fd("\33[J", STDERR_FILENO);
		vct_print_fd(vct, STDERR_FILENO);
		calc_after_totalprint(env, vct);
		ak_home(env, vct, NULL);
		while (tmpidx-- > 0)
		{
			ft_putstr_fd("\33[C", STDERR_FILENO);
			inc_x(env, 1);
			env->cursoridx++;
		}
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
		ak_arrow_left(env, vct, "\33[D");
		ft_putstr_fd("\33[K", STDERR_FILENO);
	}
	else if (env->cursoridx > 0)
	{
		vct_delchar(vct, env->cursoridx - 1);

		tmpidx = env->cursoridx;
		ak_home(env, vct, NULL);
		ft_putstr_fd("\33[J", STDERR_FILENO);
		vct_print_fd(vct, STDERR_FILENO);
		calc_after_totalprint(env, vct);
		ak_home(env, vct, NULL);
		while (tmpidx-- > 1)
		{
			ft_putstr_fd("\33[C", STDERR_FILENO);
			inc_x(env, 1);
			env->cursoridx++;
		}
	}
	return (0);
}

int8_t		ak_ctrl_d(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	if (vct_len(vct) == 0)
		exit_routine(EXIT_EOF);
	else
		ak_delete(env, vct, NULL);
	return (0);
}

int8_t		ak_ctrl_l(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)c;
	uint32_t     tmpidx;

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

int8_t		ak_ctrl_r(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)env;(void)vct;(void)c;
	return (0);
}
int8_t		ak_hightab(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
	(void)env;(void)c;
	if (completion(vct) == 0)
	{
		ak_home(env, vct, NULL);
		ft_putstr_fd("\33[J", STDERR_FILENO);
		vct_print_fd(vct, STDERR_FILENO);
		calc_after_totalprint(env, vct);
	}
	else
	{
		print_prompt(env);
		vct_print_fd(vct, STDERR_FILENO);
		calc_after_totalprint(env, vct);
	}
	return (0);
}
