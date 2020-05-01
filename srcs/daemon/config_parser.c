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
	while (sections >= 0)
	{
		if (ft_strequ(iniparser_getsecname(dict, sections), "taskmasterd") == 1)
		{
			env->opt.logfile = (char *)iniparser_getstring(dict, "taskmasterd:logfile", NULL);
			env->opt.loglevel = get_loglevel((char *)iniparser_getstring(dict, "taskmasterd:loglevel", NULL));
			env->opt.nodeamon = get_nodaemon((char *)iniparser_getstring(dict, "taskmasterd:nodaemon", NULL));
			env->opt.umask = get_umask((char *)iniparser_getstring(dict, "taskmasterd:umask", NULL));
			env->opt.user = (char *)iniparser_getstring(dict, "taskmasterd:user", NULL);
			env->opt.directory = (char *)iniparser_getstring(dict, "taskmasterd:directory", NULL);
			env->opt.childlogdir = (char *)iniparser_getstring(dict, "taskmasterd:childlogdir", NULL);
			env->opt.environ = get_environ((char *)iniparser_getstring(dict, "taskmasterd:environment", NULL));
			break ;
		}
		--sections;
	}
	// then look up program's

	// then groups => assemble linked list with program's
}
