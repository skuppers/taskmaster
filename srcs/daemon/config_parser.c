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
//	iniparser_dump(ini_dict, stdout);
	return (ini_dict);
}

int8_t	append_to_pgrmlist(t_env *env, t_program *pgrm)
{
	t_list		*list;

	list = ft_lstnew(pgrm, sizeof(t_program));
	if (env->prgm_list == NULL)
		env->prgm_list = list;
	else
		ft_lstadd(&env->prgm_list, list);
	return (0);
}

int8_t	append_to_grplist(t_env *env, t_group *grp)
{
	t_list		*list;

	list = ft_lstnew(grp, sizeof(t_group));
	if (env->goup_list == NULL)
		env->goup_list = list;
	else
		ft_lstadd(&env->goup_list, list);
	return (0);
}


char	*get_secstring(dictionary *dict, char *secname, char *key)
{
	t_vector	*buffer;
	char		*retval;

	buffer = vct_newstr(secname);
	vct_addstr(buffer, key);
	retval = (char *)iniparser_getstring(dict, vct_getstr(buffer), NULL);
	vct_del(&buffer);
	return (retval);
}

int8_t	get_secbool(dictionary *dict, char *secname, char *key)
{
	t_vector	*buffer;
	int8_t		retval;

	buffer = vct_newstr(secname);
	vct_addstr(buffer, key);
	retval = (int8_t)iniparser_getboolean(dict, vct_getstr(buffer), 0);
	vct_del(&buffer);
	return (retval);
}

int32_t	get_secint(dictionary *dict, char *secname, char *key)
{
	t_vector	*buffer;
	int32_t		retval;

	buffer = vct_newstr(secname);
	vct_addstr(buffer, key);
	return ((int32_t)iniparser_getint(dict, vct_getstr(buffer), 0));
	vct_del(&buffer);
	return (retval);
}

void	parse_ini_file(t_env *env, dictionary *dict)
{
	int32_t		sections;
	char		*secname;
	t_program	prog;
	t_group		group;

	sections = iniparser_getnsec(dict);
	print_log(env, E_LOGLVL_DEBG, "Inifile: found %d sections\n", sections);
	while (sections > 0)
	{
		--sections;
		secname = (char*)iniparser_getsecname(env->dict, sections);
		print_log(env, E_LOGLVL_DEBG, "Inifile: found section: %s\n", secname);
		if (ft_strnequ(secname, "program.", 8) == 1)
		{
			prog.name = ft_strsub(secname, 8, ft_strlen(secname) - 8);
print_log(env, E_LOGLVL_DEBG, "Inifile: found program: %s\n", prog.name);
			prog.command = get_secstring(dict, secname, ":command");
			prog.numprocs = (uint8_t)get_secint(dict, secname, ":numprocs");
			prog.directory = get_secstring(dict, secname, ":directory");
			prog.umask = (mode_t)get_secint(dict, secname, ":umask");
			prog.priority = (uint16_t)get_secint(dict, secname, ":priority");
			prog.autostart = (uint8_t)get_secbool(dict, secname, ":autostart");
			prog.autorestart = get_secstring(dict, secname, ":autorestart");
			prog.startsec = (uint16_t)get_secint(dict, secname, ":startsec");
			prog.startretries = (int8_t)get_secint(dict, secname, ":startretries");
			prog.exitcodes = get_secstring(dict, secname, ":exitcodes");
			prog.stopsignal = (uint8_t)get_secint(dict, secname, ":stopsignal");
			prog.stopwaitsec = (uint8_t)get_secint(dict, secname, ":stopwaitsec");
			prog.user = get_secstring(dict, secname, ":user");
			prog.redirect_stderr = (uint8_t)get_secbool(dict, secname, ":redirect_stderr");
			prog.stdout_logfile = get_secstring(dict, secname, ":stdout_logfile");
			prog.stderr_logfile = get_secstring(dict, secname, ":stderr_logfile");
			prog.environ = get_secstring(dict, secname, ":environment");
			append_to_pgrmlist(env, &prog);
	//		ft_strdel(&prog.name);
		}
		else if (ft_strnequ(secname, "group.", 6) == 1)
		{
			group.name = ft_strsub(secname, 6, ft_strlen(secname) - 6);
print_log(env, E_LOGLVL_DEBG, "Inifile: found group: %s\n", group.name);
			group.programs = get_secstring(dict, secname, ":programs");
			group.priority = (uint16_t)get_secint(dict, secname, ":priority");
			append_to_grplist(env, &group);
	//		ft_strdel(&group.name);
		}
		else if (ft_strequ(secname, "taskmasterd") == 0
					&& ft_strequ(secname, "taskmasterctl") == 0)
		{
			print_log(env, E_LOGLVL_ERRO, "Inifile: Unknown section: %s\n", secname);
		}
	}
}
