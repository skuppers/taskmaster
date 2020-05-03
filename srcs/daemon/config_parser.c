/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 17:12:34 by ffoissey         ###   ########.fr       */
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

t_list	*get_prgm_node(t_list *prog_list, char *name)
{
	char	*node_name;

	while (prog_list != NULL)
	{
		if (prog_list->content != NULL)
		{
			node_name = (char *)(((t_program *)(prog_list->content))->name);
			if (node_name != NULL)
			{
				if (ft_strequ(node_name, name) == TRUE)
					return (ft_lstnew_nomalloc(prog_list->content,
						sizeof(t_program)));
			}
		}
		prog_list = prog_list->next;
	}
	return (NULL);
}

void	set_grp_list(t_env *env)
{
	t_vector	*vct;
	t_vector	*split;
	t_list		*lst_grp;
	t_group		*cur_grp;
	t_list		*prgm_node;

	lst_grp = env->group_list;
	while (lst_grp != NULL)
	{
		cur_grp = (t_group *)(lst_grp->content);
		if (cur_grp != NULL || cur_grp->programs == NULL)
		{
			vct = vct_newstr(cur_grp->programs);
			cur_grp->prog_list = NULL;
			while ((split = vct_split(vct, DELIMITER_STR, NO_SEP)) != NULL)
			{
				vct_trim(split, "\t ");
				prgm_node = get_prgm_node(env->prgm_list, vct_getstr(split));
				if (prgm_node == NULL)
					 ft_dprintf(STDERR_FILENO,
							"prog %s is not set in conf file\n",
							vct_getstr(split));// print dans debug
				else
					ft_lstadd(&cur_grp->prog_list, prgm_node);
				vct_del(&split);
			}
			vct_del(&vct);
		}
		lst_grp = lst_grp->next;
	}
	/// DEBUG //////////////

	lst_grp = env->group_list;
	while (lst_grp != NULL)
	{
		cur_grp = (t_group *)(lst_grp->content);
		ft_printf("----- GROUP `%s' -----\n", cur_grp->name);
		for (t_list *tmp = cur_grp->prog_list; tmp != NULL; tmp = tmp->next)
		{
			char *name = (tmp->content == NULL) ? ""
				: ((t_program *)(tmp->content))->name;
			ft_printf("\t+--> prog: %s\n", name);
		}
		lst_grp = lst_grp->next;
	}
}

int8_t	append_to_pgrmlist(t_env *env, t_program *pgrm)
{
	t_list		*list;

	list = ft_lstnew(pgrm, sizeof(t_program));
	ft_lstadd(&env->prgm_list, list);
	return (0);
}

int8_t	append_to_grplist(t_env *env, t_group *grp)
{
	t_list		*list;

	list = ft_lstnew(grp, sizeof(t_group));
	ft_lstadd(&env->group_list, list);
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
	retval = (int32_t)iniparser_getint(dict, vct_getstr(buffer), 0);
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
	env->prgm_list = NULL;
	env->group_list = NULL;
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
		}
		else if (ft_strnequ(secname, "group.", 6) == 1)
		{
			group.prog_list = NULL;
			group.name = ft_strsub(secname, 6, ft_strlen(secname) - 6);
print_log(env, E_LOGLVL_DEBG, "Inifile: found group: %s\n", group.name);
			group.programs = get_secstring(dict, secname, ":programs");
			group.priority = (uint16_t)get_secint(dict, secname, ":priority");
			append_to_grplist(env, &group);
		}
		else if (ft_strequ(secname, "taskmasterd") == 0
					&& ft_strequ(secname, "taskmasterctl") == 0)
		{
			print_log(env, E_LOGLVL_ERRO, "Inifile: Unknown section: %s\n", secname);
		}
	}
	set_grp_list(env);
}
