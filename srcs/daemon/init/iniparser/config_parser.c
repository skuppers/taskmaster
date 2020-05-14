/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/13 17:15:07 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

int8_t			append_to_pgrmlist(t_denv *env, t_program *pgrm)
{
	t_list		*list;
	char		**avs;

	avs = ft_strsplit(pgrm->command, ' ');
	pgrm->bin = avs[0];
	pgrm->avs = avs;
	list = ft_lstnew(pgrm, sizeof(t_program));
	ft_lstadd(&env->prgm_list, list);
	return (0);
}

int8_t		is_in_range(int32_t i, int32_t min, int32_t max)
{
	if (i < min || i > max)
		return (FALSE);
	return (TRUE);
}

void	get_new_prog(t_denv *env, dictionary *dict, char *secname)
{
	t_program	prog;
	uint8_t		error;

	error = 0;
	ft_memset(&prog, 0, sizeof(t_program));
	prog.name = ft_strsub(secname, 8, ft_strlen(secname) - 8);
	prog.command =  get_command(&error, dict, secname);
	prog.numprocs = get_numprocs(&error, dict, secname);
	prog.autostart = (uint8_t)get_secbool(dict, secname, ":autostart");
	prog.autorestart = get_autorestart(&error, dict, secname);
	prog.startsecs = get_startsecs(&error, dict, secname);
	prog.startretries = get_startretries(&error, dict, secname);
	prog.stopwaitsecs = get_stopwaitsecs(&error, dict, secname);
	get_exitcodes(&error, &prog, dict, secname);
	prog.stopsignal = get_stopsig(&error, dict, secname);
	prog.userid = get_userid(&error, dict, secname);
	prog.directory = get_directory(&error, dict, secname);
	prog.priority = get_priority(&error, dict, secname);
	prog.stdout_logfile = get_stdoutlog(&error, dict, secname);
	prog.stderr_logfile = get_stderrlog(&error, dict, secname);
	prog.environ = get_environement(&error, dict, secname);
	prog.umask = get_umask(&error, dict, secname);
	if (error == 1)
	{
		dprintf(STDERR_FILENO, "taskmasterd: One more more errors happened "
			"while parsing the configuration file.\n");
		exit_routine(E_LOGLVL_ERRO, NULL);
	}
	prog.env = NULL;
	prog.availmode = E_LOCKED;
	strvalue_to_lst(&prog.env, prog.environ);
	append_to_pgrmlist(env, &prog);
	tlog(E_LOGLVL_DEBG, "Inifile: parsed program: %s\n", prog.name);
}

void			parse_ini_file(void)
{
	int32_t		sections;
	char		*secname;

	sections = iniparser_getnsec(g_denv->dict);
	g_denv->prgm_list = NULL;
	tlog(E_LOGLVL_DEBG, "Inifile: found %d sections\n", sections);
	while (sections > 0)
	{
		--sections;
		secname = (char*)iniparser_getsecname(g_denv->dict, sections);
		tlog(E_LOGLVL_DEBG, "Inifile: found section: %s\n", secname);

		if (ft_strnequ(secname, "program.", 8) == TRUE)
			get_new_prog(g_denv, g_denv->dict, secname);
			
		else if (ft_strequ(secname, "taskmasterd") == FALSE
					&& ft_strequ(secname, "taskmasterctl") == FALSE)
			tlog(E_LOGLVL_ERRO,
				"Inifile: Unknown section: %s\n", secname);
	}
}
