/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 18:40:01 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

int8_t	append_to_pgrmlist(t_env *env, t_program *pgrm)
{
	t_list		*list;
	char		**avs;

	pgrm->instance = NULL;
	avs = ft_strsplit(pgrm->command, ' ');
	pgrm->bin = avs[0];
	pgrm->avs = avs;
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

void		get_exitcodes(t_program *prog, dictionary *dict, char *secname)
{
	char	*to_split;

	to_split = get_secstring(dict, secname, ":exitcodes");
	if (to_split == NULL)
		to_split = "0";
	prog->exitcodes = ft_strsplit(to_split, ',');
}

uint8_t		get_autorestart(dictionary *dict, char *secname)
{
	char	*str;

	str = get_secstring(dict, secname, ":autorestart");
	if (ft_strequ(str, "true") == 1)
		return (TRUE);
	if (ft_strequ(str, "false") == 1)
		return (FALSE);
	if (ft_strequ(str, "unexpected") == 1)
		return (UNEXPECTED);
	dprintf(STDERR_FILENO, "Autorestart: Unknown string: %s, defaulting to false\n", str);
	return (FALSE);
}


static void	get_new_prog(t_env *env, dictionary *dict, char *secname)
{
	t_program	prog;

	prog.name = ft_strsub(secname, 8, ft_strlen(secname) - 8);
	prog.command = get_secstring(dict, secname, ":command");
	prog.numprocs = (uint8_t)get_secint(dict, secname, ":numprocs");
	prog.directory = get_secstring(dict, secname, ":directory");
	prog.umask = (mode_t)get_secint(dict, secname, ":umask");
	prog.priority = (uint16_t)get_secint(dict, secname, ":priority");
	prog.autostart = (uint8_t)get_secbool(dict, secname, ":autostart");
	prog.autorestart = get_autorestart(dict, secname);
	prog.startsec = get_secint(dict, secname, ":startsec");
	prog.startretries = (int8_t)get_secint(dict, secname, ":startretries");
	get_exitcodes(&prog, dict, secname);
	prog.stopsignal = (uint8_t)get_secint(dict, secname, ":stopsignal");
	prog.stopwaitsec = (uint8_t)get_secint(dict, secname, ":stopwaitsec");
	prog.user = get_secstring(dict, secname, ":user");
	prog.redirect_stderr = (uint8_t)get_secbool(dict, secname,
													":redirect_stderr");
	prog.stdout_logfile = get_secstring(dict, secname, ":stdout_logfile");
	prog.stderr_logfile = get_secstring(dict, secname, ":stderr_logfile");
	prog.environ = get_secstring(dict, secname, ":environment");
	prog.env = NULL;
	strvalue_to_lst(&prog.env, prog.environ);
	append_to_pgrmlist(env, &prog);
	print_log(env, E_LOGLVL_DEBG, "Inifile: found program: %s\n", prog.name);
}

static void	get_new_group(t_env *env, dictionary *dict, char *secname)
{
	t_group		group;

	group.prog_list = NULL;
	group.name = ft_strsub(secname, 6, ft_strlen(secname) - 6);
	group.programs = get_secstring(dict, secname, ":programs");
	group.priority = (uint16_t)get_secint(dict, secname, ":priority");
	append_to_grplist(env, &group);
	print_log(env, E_LOGLVL_DEBG, "Inifile: found group: %s\n", group.name);
}

void			parse_ini_file(t_env *env, dictionary *dict)
{
	int32_t		sections;
	char		*secname;

	sections = iniparser_getnsec(dict);
	env->prgm_list = NULL;
	env->group_list = NULL;
	print_log(env, E_LOGLVL_DEBG, "Inifile: found %d sections\n", sections);
	while (sections > 0)
	{
		--sections;
		secname = (char*)iniparser_getsecname(env->dict, sections);
		print_log(env, E_LOGLVL_DEBG, "Inifile: found section: %s\n", secname);

		if (ft_strnequ(secname, "program.", 8) == TRUE)
			get_new_prog(env, dict, secname);

		else if (ft_strnequ(secname, "group.", 6) == TRUE)
			get_new_group(env, dict, secname);
			
		else if (ft_strequ(secname, "taskmasterd") == FALSE
					&& ft_strequ(secname, "taskmasterctl") == FALSE)
			print_log(env, E_LOGLVL_ERRO,
				"Inifile: Unknown section: %s\n", secname);
	}
	set_grp_list(env);
}
