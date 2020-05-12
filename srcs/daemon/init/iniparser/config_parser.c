/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 17:06:06 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

int8_t	append_to_pgrmlist(t_denv *env, t_program *pgrm)
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

void		get_exitcodes(uint8_t *err, t_program *prog, dictionary *dict, char *secname)
{
	int 	i;
	char	*to_split;

	to_split = get_secstring(dict, secname, ":exitcodes");
	if (to_split == NULL)
		to_split = "0";
	prog->exitcodes = ft_strsplit(to_split, ',');
	i = 0;
	while (prog->exitcodes[i] != NULL)
	{
		if (ft_strlen(prog->exitcodes[i]) > 3)
		{
			*err = 1;
			dprintf(STDERR_FILENO, "taskmasterd: [%s] - exitcodes is not in range 0-255\n", secname);
			return ;
		}
		else if (ft_atoi(prog->exitcodes[i]) > 255
				|| ft_atoi(prog->exitcodes[i]) < 0)
		{
			*err = 1;
			dprintf(STDERR_FILENO, "taskmasterd: [%s] - exitcodes is not in range 0-255\n", secname);
			return ;
		}
		++i;
	}
}

uint8_t		get_autorestart(uint8_t *error, dictionary *dict, char *secname)
{
	char	*str;

	str = get_secstring(dict, secname, ":autorestart");
	if (str == NULL)
		return (UNEXPECTED);
	if (ft_strequ(str, "true") || ft_strequ(str, "TRUE") || ft_strequ(str, "1"))
		return (TRUE);
	if (ft_strequ(str, "false") || ft_strequ(str, "FALSE") || ft_strequ(str, "0"))
		return (FALSE);
	if (ft_strequ(str, "unexpected") || ft_strequ(str, "UNEXPECTED"))
		return (UNEXPECTED);
	dprintf(STDERR_FILENO, "taskmasterd: [%s] - autorestart does not match known instructions:"
							" TRUE / FALSE / UNEXPECTED\n", secname);
	*error = 1;
	return (FALSE);
}

int8_t		is_in_range(int32_t i, int32_t min, int32_t max)
{
	if (i < min || i > max)
		return (FALSE);
	return (TRUE);
}	

uint8_t		get_numprocs(uint8_t *error, dictionary *d, char *name)
{
	int32_t		get;

	get = get_secint(d, name, ":numprocs", 0);
	if (get == 0)
		return (1);
	if (!is_in_range(get, 1, 255))
	{
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - numprocs is not in range 1-255: %d\n", name, get);
		*error = 1;
		return (0);
	}
	return ((uint8_t)get);
}

char		*get_command(uint8_t *error, dictionary *d, char *name)
{
	char	*cmd;

	cmd = get_secstring(d, name, ":command");
	if (cmd == NULL || ft_strlen(cmd) == 0)
	{
		dprintf(STDERR_FILENO, "[%s] - You need to specify a command!\n", name);
		*error = 1;
		return (NULL);
	}
	return (cmd);
}

uint8_t		get_startsecs(uint8_t *error, dictionary *d, char *name)
{
	int32_t get;

	get = get_secint(d, name, ":startsecs", 0);
	if (get == 0)
		get = 1;
	if (!is_in_range(get, 1, 255))
	{
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - startsecs is not in range: 1-255\n", name);
		*error = 1;
		return (0);
	}
	return ((uint8_t)get);
}

uint8_t		get_startretries(uint8_t *err, dictionary *d, char *name)
{
	int32_t get;
	
	get = get_secint(d, name, ":startretries", 0);
	if (get == 0)
		get = 3;
	if (!is_in_range(get, 1, 255))
	{
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - startretries is not in range: 1-255\n", name);
		*err = 1;
		return (0);
	}
	return ((uint8_t)get);
}

uint8_t		get_stopwaitsecs(uint8_t *err, dictionary *d, char *name)
{
	int32_t get;
	
	get = get_secint(d, name, ":stopwaitsecs", 0);
	if (get == 0)
		get = 1;
	if (!is_in_range(get, 1, 255))
	{
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - stopwaitsecs is not in range: 1-255\n", name);
		*err = 1;
		return (0);
	}
	return ((uint8_t)get);
}

uint8_t		get_stopsig(uint8_t *err, dictionary *d, char *name)
{
	char		*get;
	int			i;
	static char	*sigs[] = {"TERM", "HUP","INT", "QUIT", "KILL", "USR1","USR2"};
	static int	signo[] = {SIGTERM, SIGHUP, SIGINT, SIGQUIT, SIGKILL, SIGUSR1, SIGUSR2};

	get = get_secstring(d, name, ":stopsignal");
	if (get == NULL)
		get = sigs[0];
	i = 0;
	while (i < 7)
	{
		if (ft_strequ(get, sigs[i]) == TRUE)
			return (signo[i]);
		++i;
	}
	dprintf(STDERR_FILENO, "taskmasterd: [%s] - stopsignal must be one of : "
			"TERM, HUP, INT, QUIT, KILL, USR1, or USR2.\n", name);
	*err = 1;
	return (0);
}

int16_t		get_userid(uint8_t *err, dictionary *d, char *name)
{
	char	*get;

	get = get_secstring(d, name, ":userid");
	if (get == NULL)
		return (-1);
	if (ft_strlen(get) == 0)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - userid field cannot be blank.\n", name);
		return (-1);
	}
	if (ft_strcheck(get, ft_isdigit) != TRUE)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - userid field must be numeric.\n", name);
		return (-1);
	}
	if (ft_strlen(get) >= 6 || ft_atoi(get) < 0 || ft_atoi(get) > 16535)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] -  userid is not in range 0-16535\n", name);
		return (-1);
	}
	return (ft_atoi(get));
}

char	*get_directory(uint8_t *err, dictionary *d, char *name)
{
	char *get;

	get = get_secstring(d, name, ":directory");
	if (get == NULL)
		return (NULL);
	if (ft_strlen(get) == 0)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - directory field cannot be blank.\n", name);
		return (NULL);
	}
	return (get);
}

uint16_t		get_priority(uint8_t *err, dictionary *d, char *name)
{
	int32_t get;
	
	get = get_secint(d, name, ":priority", 0);
	if (get == 0)
		get = 999;
	if (!is_in_range(get, 1, 999))
	{
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - priority is not in range: 1-999\n", name);
		*err = 1;
		return (0);
	}
	return ((uint16_t)get);
}

char	*get_stdoutlog(uint8_t *err, dictionary *d, char *name)
{
	char *get;

	get = get_secstring(d, name, ":stdout_logfile");
	if (get == NULL)
		return ("AUTO");
	if (ft_strlen(get) == 0)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - stdout_logfile field cannot be blank.\n", name);
		return (NULL);
	}
	return (get);
}

char	*get_stderrlog(uint8_t *err, dictionary *d, char *name)
{
	char *get;

	get = get_secstring(d, name, ":stderr_logfile");
	if (get == NULL)
		return ("AUTO");
	if (ft_strlen(get) == 0)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - stderr_logfile field cannot be blank.\n", name);
		return (NULL);
	}
	return (get);
}

char	*get_environement(uint8_t *err, dictionary *d, char *name)
{
	char *get;

	get = get_secstring(d, name, ":environment");
	if (get == NULL)
		return (NULL);
	if (ft_strlen(get) == 0)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - environment field cannot be blank.\n", name);
		return (NULL);
	}
	return (get);
}

mode_t		get_umask(uint8_t *err, dictionary *d, char *name)
{
	char	*get;

	get = get_secstring(d, name, ":umask");
	if (get == NULL)
		return ((mode_t)strtol("0", NULL, 8));
	if (ft_strlen(get) == 0)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - umask field cannot be blank.\n", name);
		return (0);
	}
	if (ft_strcheck(get, ft_isdigit) != TRUE)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [taskmasterd] - umask field must be numeric.\n");
	}
	return ((mode_t)strtol(get, NULL, 8));
}

void	get_new_prog(t_denv *env, dictionary *dict, char *secname)
{
	t_program	prog;
	uint8_t		error;

	error = 0;
	prog.name = ft_strsub(secname, 8, ft_strlen(secname) - 8);
	
	prog.command =  get_command(&error, dict, secname);
	prog.numprocs = get_numprocs(&error, dict, secname);
	prog.autostart = (uint8_t)get_secbool(dict, secname, ":autostart");;
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
		dprintf(STDERR_FILENO, "taskmasterd: One more more errors happened while parsing the configuration file.\n");
		exit_routine(ERR, NULL);
	}
	prog.env = NULL;
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
