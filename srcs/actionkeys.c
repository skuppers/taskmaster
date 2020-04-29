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

int8_t		ak_arrow_up(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
    (void)env;(void)vct;(void)c;
    return (0);
}

int8_t		ak_arrow_down(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
    (void)env;(void)vct;(void)c;
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
    (void)env;(void)vct;(void)c;
    return (0);
}

int8_t		ak_end(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
    (void)env;(void)vct;(void)c;
    return (0);
}

int8_t		ak_delete(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
    (void)env;(void)vct;(void)c;
    return (0);
}

int8_t		ak_backspace(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
    (void)env;(void)vct;(void)c;
    return (0);
}

int8_t		ak_ctrl_d(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
    (void)env;(void)vct;(void)c;
    return (0);
}
int8_t		ak_ctrl_l(t_env *env, t_vector *vct, char c[BUFF_SIZE])
{
    (void)env;(void)vct;(void)c;
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