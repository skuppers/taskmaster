/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ini.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 13:21:56 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/09 20:49:15 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void free_inifile(dictionary *dict)
{
	iniparser_freedict(dict);
}

dictionary *parse_inifile(char *str)
{
    dictionary	*ini_dict;

	ini_dict = iniparser_load(str);
	if (ini_dict == NULL)
	{
		dprintf(STDERR_FILENO, "ERROR: Could not read ini file\n");
		exit_routine(ERR, strerror(errno));
	}
	//iniparser_dump(ini_dict, stdout);
	return (ini_dict);
}
