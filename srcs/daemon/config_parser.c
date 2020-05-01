/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 19:05:55 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

void free_inifile(dictionary *dict)
{
	iniparser_freedict(dict);
}

dictionary *load_ini_file(char *str)
{
    dictionary	*ini_dict;

	ini_dict = iniparser_load(str);
	if (ini_dict == NULL)
	{
		printf("Could not read ini file: %s\n", strerror(errno));
        exit_routine();
	}
	//iniparser_dump(ini_dict, stdout);
	return (ini_dict);
}

void	parse_ini_file(t_env *env, dictionary *dict)
{
	uint32_t	sections;

	sections = iniparser_getnsec(dict);
	// look for taskmasterd
	while (sections >= 0)
	{
		if (ft_strequ(iniparser_getsecname(dict, sections), "taskmasterd") == 1)
		{
			char *tmp = (char *)iniparser_getstring(dict, "taskmasterd:logfile", NULL);
		}
	}
	// then look up program's

	// then groups => assemble linked list with program's
}
