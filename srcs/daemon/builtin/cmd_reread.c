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

//t_denv	*g_tmpenv;

int		_strcmp(const char *s1, const char *s2)
{
	size_t		i;

	i = 0;
	if (s1 == NULL && s2 != NULL)
		return (1);
	if (s1 != NULL && s2 == NULL)
		return (1);
	if (s1 == NULL && s2 == NULL)
		return (0);
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((int)((unsigned char)s1[i] - (unsigned char)s2[i]));
}

static int32_t	compare_prog(t_program *op, t_program *np)
{
	int diff;

	diff = 0;
	diff += _strcmp(op->command, np->command);
	diff += _strcmp(op->directory, np->directory);
	diff += _strcmp(op->stdout_logfile, np->stdout_logfile);
	diff += _strcmp(op->stderr_logfile, np->stderr_logfile);
	diff += _strcmp(op->environ, np->environ);
	diff += (op->umask != np->umask) ? 1 : 0;
	diff += (op->priority != np->priority) ? 1 : 0;
	diff += (op->startsecs != np->startsecs) ? 1 : 0;
	diff += (op->stopwaitsecs != np->stopwaitsecs) ? 1 : 0;
	diff += (op->numprocs != np->numprocs) ? 1 : 0;
	diff += (op->stopsignal != np->stopsignal) ? 1 : 0;
	diff += (op->autostart != np->autostart) ? 1 : 0;
	diff += (op->startretries != np->startretries) ? 1 : 0;
	diff += (op->autorestart != np->autorestart) ? 1 : 0;
	//EXITCODES
	return (diff);
}

static int8_t		check_daemon_opts(dictionary *d, int sec)
{
	(void)sec;
	char	*tmp;

	if (ft_strequ(iniparser_getstring(d, "taskmasterd:logfile", NULL),
									g_denv->opt.str[LOGFILE]) == FALSE)
		return (FAILURE);
/*	if (ft_strequ(iniparser_getstring(d, "taskmasterd:loglevel", NULL),
									g_denv->opt.str[LOGLEVEL]) == FALSE)
		return (FAILURE);*/
	if (ft_strequ(iniparser_getstring(d, "taskmasterd:userid", NULL),
									g_denv->opt.str[USER]) == FALSE)
		return (FAILURE);
	if (ft_strequ(iniparser_getstring(d, "taskmasterd:directory", NULL),
									g_denv->opt.str[DIRECTORY]) == FALSE)
		return (FAILURE);
	if (ft_strequ(iniparser_getstring(d, "taskmasterd:childlogdir", NULL),
									g_denv->opt.str[CHILDLOGDIR]) == FALSE)
		return (FAILURE);
	if (ft_strequ(iniparser_getstring(d, "taskmasterd:environment", NULL),
									g_denv->opt.environ) == FALSE)
		return (FAILURE);

	tmp = (char *)iniparser_getstring(d, "taskmasterd:umask", NULL);
	if (tmp != NULL)
	{
		if (strtol(tmp, NULL, 8) != g_denv->opt.umask)
			return (FAILURE);
	}
	
//	if (get_nodaemon(iniparser_getstring(d, "taskmasterd:nodaemon", NULL))
//		!= )
	return (SUCCESS);
}

void			reparse_ini_file(dictionary *d, t_denv *tmpenv)
{
	int32_t		sections;
	char		*secname;

	sections = iniparser_getnsec(d);
	tlog(E_LOGLVL_DEBG, "Inifile: found %d sections\n", sections);
	while (sections > 0)
	{
		--sections;
		secname = (char*)iniparser_getsecname(d, sections);
		tlog(E_LOGLVL_DEBG, "Inifile: found section: %s\n", secname);

		if (ft_strnequ(secname, "program.", 8) == TRUE)
			get_new_prog(tmpenv, d, secname);
			
		else if (ft_strequ(secname, "taskmasterd") == FALSE
					&& ft_strequ(secname, "taskmasterctl") == FALSE)
			tlog(E_LOGLVL_ERRO,
				"Inifile: Unknown section: %s\n", secname);
	}
}

static void		check_removed(t_denv *nenv)
{
	t_list		*orig_ptr;
	t_program	*orig_prg;
	t_list		*new_ptr;
	t_program	*new_prg;
	int			found;

	orig_ptr = g_denv->prgm_list;
	while (orig_ptr != NULL)
	{
		found = 0;
		orig_prg = orig_ptr->content;
		new_ptr = nenv->prgm_list;
		while (new_ptr != NULL && found == 0)
		{
			new_prg = new_ptr->content;
			if (ft_strequ(orig_prg->name, new_prg->name) == TRUE)
				found = 1;
			new_ptr = new_ptr->next;
		}
		if (found == 0)
			orig_prg->availmode = E_REMOVED;
		orig_ptr = orig_ptr->next;
	}
}

static void		check_added(t_denv *nenv)
{
	t_list		*orig_ptr;
	t_program	*orig_prg;
	t_list		*new_ptr;
	t_program	*new_prg;
	int			found;

	new_ptr = nenv->prgm_list;
	while (new_ptr != NULL)
	{
		found = 0;
		new_prg = new_ptr->content;
		orig_ptr = g_denv->prgm_list;
		while (orig_ptr != NULL && found == 0)
		{
			orig_prg = orig_ptr->content;
			if (ft_strequ(new_prg->name, orig_prg->name) == TRUE)
				found = 1;
			orig_ptr = orig_ptr->next;
		}
		if (found == 0)
			new_prg->availmode = E_ADDED;
		new_ptr = new_ptr->next;
	}
}

static void		check_modified(t_denv *nenv)
{
	t_list		*orig_ptr;
	t_program	*orig_prg;
	t_list		*new_ptr;
	t_program	*new_prg;

	orig_ptr = g_denv->prgm_list;
	while (orig_ptr != NULL)
	{
		orig_prg = orig_ptr->content;
		new_ptr = nenv->prgm_list;
		while (new_ptr != NULL)
		{
			new_prg = new_ptr->content;
			if (ft_strequ(orig_prg->name, new_prg->name) == TRUE)
				if (orig_prg->availmode == E_LOCKED && compare_prog(orig_prg, new_prg) != 0)
					orig_prg->availmode = E_CHANGED;
			new_ptr = new_ptr->next;
		}
		orig_ptr = orig_ptr->next;
	}
}

static t_vector	*register_changes(t_denv *env)
{
	t_vector	*resp;

	resp = vct_newstr("");

	check_removed(env);
	check_added(env);
	check_modified(env);

	for (t_list *op = g_denv->prgm_list; op != NULL; op = op->next)
	{
		t_program *tmp = op->content;
		if (tmp->availmode == E_REMOVED)
		{
			vct_addstr(resp, tmp->name);
			vct_addstr(resp, " removed\n");
		}
		else if (tmp->availmode == E_CHANGED)
		{
			vct_addstr(resp, tmp->name);
			vct_addstr(resp, " modified\n");
		}
	}
	for (t_list *op = env->prgm_list; op != NULL; op = op->next)
	{
		t_program *tmp = op->content;
		if (tmp->availmode == E_ADDED)
		{
			vct_addstr(resp, tmp->name);
			vct_addstr(resp, " added\n");
			append_to_pgrmlist(g_denv, tmp);
		}
	}
	return (resp);
}

t_vector		*reread_file(t_instance *in, t_program *prg)
{
	(void)in;(void)prg;
	dictionary	*dict;
	int			sections;
	t_denv		*tmpenv;

	if (g_tmpenv != NULL)
	{
		ft_lstdel(&g_tmpenv->prgm_list, del_prgm);
		if (g_tmpenv->dict != NULL)
	    	free_inifile(g_tmpenv->dict);
		free(g_tmpenv);
	}
	tmpenv = malloc(sizeof(t_denv));
	ft_bzero(tmpenv, sizeof(t_denv));
	g_tmpenv = tmpenv;
	dict = iniparser_load(g_denv->opt.str[CONFIGURATION]);
	if (dict == NULL)
		return (vct_newstr("reread: could not open file to read.\n"));
	g_tmpenv->dict = dict;
	sections = iniparser_getnsec(dict);
	while (sections >= 0)
	{
		if (ft_strequ(iniparser_getsecname(dict, sections), "taskmasterd") == true)
		{
			if (check_daemon_opts(dict, sections) == FAILURE)
				return (vct_newstr("reread: You cannot change the"
							" taskmasterd section at runtime.\n")); // Can not change taskmasterd config without restart
			break ;
		}
		--sections;
	}
	if (sections < 0)
		return (vct_newstr("taskmasterd: [taskmasterd] section is not defined.\n"));
	reparse_ini_file(dict, tmpenv);
	return (register_changes(tmpenv));
}

t_vector			*cmd_reread(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x00)
		vct = reread_file(NULL, NULL);
	return (vct);
}
