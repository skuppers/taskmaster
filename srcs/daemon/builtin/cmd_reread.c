/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_reread.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:32:52 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 11:19:28 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

int		do_strcmp(const char *s1, const char *s2)
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

static uint8_t	exitcode_changed(t_program *op, t_program *np)
{
	int	i;

	i = 0;
	while (op->exitcodes[i] != NULL)
	{
		if (ft_strequ(op->exitcodes[i], np->exitcodes[i]) == FALSE)
			return (1);
		++i;
	}
	return (0);
}

static int32_t	compare_prog(t_program *op, t_program *np)
{
	int diff;

	diff = 0;
	diff += do_strcmp(op->command, np->command);
	diff += do_strcmp(op->directory, np->directory);
	diff += do_strcmp(op->stdout_logfile, np->stdout_logfile);
	diff += do_strcmp(op->stderr_logfile, np->stderr_logfile);
	diff += do_strcmp(op->environ, np->environ);
	diff += (op->userid != np->userid) ? 1 : 0;
	diff += (op->umask != np->umask) ? 1 : 0;
	diff += (op->priority != np->priority) ? 1 : 0;
	diff += (op->startsecs != np->startsecs) ? 1 : 0;
	diff += (op->stopwaitsecs != np->stopwaitsecs) ? 1 : 0;
	diff += (op->numprocs != np->numprocs) ? 1 : 0;
	diff += (op->stopsignal != np->stopsignal) ? 1 : 0;
	diff += (op->autostart != np->autostart) ? 1 : 0;
	diff += (op->startretries != np->startretries) ? 1 : 0;
	diff += (op->autorestart != np->autorestart) ? 1 : 0;
	diff += (exitcode_changed(op, np)) ? 1 : 0;
	return (diff);
}

static int8_t		check_daemon_opts(dictionary *d, int sec)
{
	(void)sec;
	char	*tmp;
	bool	yn;
	bool	is_nodea;

	if (ft_strequ(iniparser_getstring(d, "taskmasterd:logfile", NULL),
									g_denv->opt.str[LOGFILE]) == FALSE)
		return (FAILURE);
	if (ft_strequ(iniparser_getstring(d, "taskmasterd:loglevel", NULL),
									g_denv->opt.str[LOGLEVEL]) == FALSE)
		return (FAILURE);
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
	is_nodea = (bool)(g_denv->opt.optmask & OPT_NODAEMON);
	yn = get_nodaemon((char *)iniparser_getstring(d,
							 "taskmasterd:nodaemon", NULL));
	dprintf(2, "old=%d new=%d\n", is_nodea, yn);
	if (yn != is_nodea)
		return (FAILURE);
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

static void		activate_instances(t_program *prg)
{
	t_instance *inst;
	uint8_t		inst_nb;

	inst_nb = 0;
	while (inst_nb < prg->numprocs)
	{
		inst = new_instance(inst_nb, prg->name);
		if (inst == NULL)
		{
			tlog(E_LOGLVL_ERRO, "Failed to allocate instance\n");
			break ;
		}
		add_instance(prg, inst);
		++inst_nb;
	}
}

static void		register_added(t_denv *env, t_vector *v)
{
	t_list		*ptr;
	t_program	*prg;

	ptr = env->prgm_list;
	while (ptr != NULL)
	{
		prg = ptr->content;
		if (prg->availmode == E_ADDED)
		{
			activate_instances(prg);
			vct_addstr(v, prg->name);
			vct_addstr(v, " available\n");
		}
		ptr = ptr->next;
	}
}

static void		register_modified(t_denv *env, t_vector *v)
{
	t_list		*ptr;
	t_program	*prg;

	ptr = env->prgm_list;
	while (ptr != NULL)
	{
		prg = ptr->content;
		if (prg->availmode == E_REMOVED)
		{
			vct_addstr(v, prg->name);
			vct_addstr(v, " disappeared\n");
		}
		else if (prg->availmode == E_CHANGED)
		{
			vct_addstr(v, prg->name);
			vct_addstr(v, " changed\n");
		}
		ptr = ptr->next;
	}
}

static t_vector	*register_changes(t_denv *env)
{
	t_vector	*resp;

	resp = vct_newstr("");
	check_removed(env);
	check_added(env);
	check_modified(env);
	register_added(env, resp);
	register_modified(g_denv, resp);
	return (resp);
}

static dictionary	*load_dict(void)
{
	dictionary	*dict;

	dict = iniparser_load(g_denv->opt.str[CONFIGURATION]);
	if (dict == NULL)
		return (NULL);
	g_tmpenv->dict = dict;
	return (dict);
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
		free(g_tmpenv);
	}
	tmpenv = malloc(sizeof(t_denv));
	ft_bzero(tmpenv, sizeof(t_denv));
	g_tmpenv = tmpenv;

	if ((dict = load_dict()) == NULL)
		return (vct_newstr("reread: could not open file to read.\n"));
	sections = iniparser_getnsec(dict);
	while (sections >= 0)
	{
		if (ft_strequ(iniparser_getsecname(dict, sections), "taskmasterd") == true)
		{
			if (check_daemon_opts(dict, sections) == FAILURE)
				return (vct_newstr("reread: You cannot change the"
							" taskmasterd section at runtime.\n"));
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
