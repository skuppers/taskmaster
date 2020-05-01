/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termmode.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/01 13:03:08 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void        create_termmode(t_env *environment)
{
    struct termios  term;
    
    environment->orig = malloc(sizeof(struct termios));
    if (tcgetattr(STDIN_FILENO, &term) != 0)
		return ;
    ft_memcpy(environment->orig, &term, sizeof(struct termios));

    term.c_lflag &= ~TOSTOP;
	term.c_lflag &= ~(ICANON);
	term.c_lflag &= ~(ECHO);
	term.c_lflag |= ISIG;
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;

    environment->taskmst = malloc(sizeof(struct termios));
    if (environment->taskmst != NULL)
		ft_memcpy(environment->taskmst, &term, sizeof(struct termios));
}

uint8_t    set_termmode(t_env *environment)
{
	if (environment->taskmst != NULL)
	{
    	if (tcsetattr(STDIN_FILENO, TCSADRAIN, environment->taskmst) != 0)
			return (-1);
	}
    return (0);
}

void    release_termmode(t_env *environment)
{
	if (environment->orig != NULL)
	{
    	if (tcsetattr(STDIN_FILENO, TCSADRAIN, environment->orig) != 0)
			return ;
	}
}
