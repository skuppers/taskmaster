/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:11:01 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/15 17:11:06 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

char			*loglvl_todefine(uint8_t loglvl)
{
	static char	*str_log[] = {LOG_CRIT_STR, LOG_ERRO_STR, LOG_WARN_STR,
								LOG_INFO_STR, LOG_DEBG_STR};

	return ((loglvl < 5) ? str_log[loglvl] : LOG_DEBG_STR);
}

char			*loglvl_tostr(uint8_t loglvl)
{
	static char	*str_log[] = {LOGLVL_CRIT, LOGLVL_ERRO, LOGLVL_WARN,
								LOGLVL_INFO, LOGLVL_DEBG};

	return ((loglvl < 5) ? str_log[loglvl] : LOGLVL_DEBG);
}

uint8_t			get_loglevel(char *str)
{
	uint8_t		loglvl;

	loglvl = 0;
	while (loglvl < 5)
	{
		if (ft_strequ(str, loglvl_tostr(loglvl)) == true)
			return (loglvl);
		loglvl++;
	}
	return (E_LOGLVL_DEBG);
}
