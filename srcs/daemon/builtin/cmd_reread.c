/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_reread.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:32:52 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 02:33:02 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static uint8_t		reread_file(void)
{
	dictionary	*dict;
	int			sections;

	dict = iniparser_load(g_denv->opt.str[CONFIGURATION]);
	sections = iniparser_getnsec(dict);
	while (sections >= 0)
	{
		if (ft_strequ(iniparser_getsecname(g_denv->dict, sections),
					"taskmasterd") == true)
		{
			taskmasterd_override(g_denv, g_denv->dict);
			return ;
		}
		--sections;
	}
	if (sections < 0)
		dprintf(STDERR_FILENO, "taskmasterd: [taskmasterd]"
								" section is not defined.\n");
}

t_vector			*cmd_reread(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}
