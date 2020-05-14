/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoisssey@student.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/11 14:13:28 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 23:04:27 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

uint64_t		compute_mask(char c[BUFF_SIZE])
{
	uint16_t	shift;
	uint16_t	index;
	uint64_t	value;
	uint64_t	tmp;

	shift = 56;
	index = 0;
	value = 0;
	while (index < 8 && c[index] != '\0')
	{
		tmp = c[index++];
		tmp <<= shift;
		value |= tmp;
		shift -= 8;
	}
	return (value);
}

void			handle_actionkey(t_env *env, char c[BUFF_SIZE], t_vector *vct)
{
	uint32_t	index;
	uint64_t	value;

	index = 0;
	value = compute_mask(c);
	while (index < AK_AMOUNT)
	{
		if (value == env->ak_masks[index])
			env->actionkeys[index](env, vct, c);
		++index;
	}
}

int8_t			put_newline(t_vector *dest, char *src, size_t size)
{
	if (ft_strequ(src, "\n") == 1)
	{
		ft_putchar_fd('\n', STDIN_FILENO);
		if (vct_addnstr(dest, src, size) == FAILURE)
			return (FAILURE);
	}
	return (0);
}
