/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 18:44:25 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "client_taskmaster.h"

void        update_winsize(t_env *env)
{
    struct winsize w;

    ioctl(STDERR_FILENO, TIOCGWINSZ, &w);
    env->winhei = w.ws_row;
    env->winwid = w.ws_col;
}