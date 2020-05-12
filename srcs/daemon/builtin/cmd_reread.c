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

t_denv				*g_newenv;

static int8_t		check_daemon_opts(dictionary *d, int sec)
{
	(void)sec;

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
	if (strtol(iniparser_getstring(d, "taskmasterd:umask", NULL), NULL, 8)
									!= g_denv->opt.umask)
		return (FAILURE);
//	if (get_nodaemon(iniparser_getstring(d, "taskmasterd:nodaemon", NULL))
//		!= )
	return (SUCCESS);
}

void			reparse_ini_file(dictionary *d)
{
	int32_t		sections;
	char		*secname;

	sections = iniparser_getnsec(d);
	g_newenv->prgm_list = NULL;
	tlog(E_LOGLVL_DEBG, "Inifile: found %d sections\n", sections);

	while (sections > 0)
	{
		--sections;
		secname = (char*)iniparser_getsecname(d, sections);
		tlog(E_LOGLVL_DEBG, "Inifile: found section: %s\n", secname);

		if (ft_strnequ(secname, "program.", 8) == TRUE)
			get_new_prog(g_newenv, d, secname);
			
		else if (ft_strequ(secname, "taskmasterd") == FALSE
					&& ft_strequ(secname, "taskmasterctl") == FALSE)
			tlog(E_LOGLVL_ERRO,
				"Inifile: Unknown section: %s\n", secname);
	}
}

static t_vector	*register_changes(void)
{
	t_list		*orig_ptr;
	t_program	*orig_prg;

	t_list		*new_ptr;
	t_program	*new_prg;

	int			found;
	t_vector	*resp;
	char		*tmp;

	resp = vct_newstr("");
	orig_ptr = g_denv->prgm_list;

	//Check removed/modified
	while (orig_ptr != NULL)
	{
		found = 0;
		orig_prg = orig_ptr->content; // Get current prgs once/once
		
		new_ptr = g_newenv->prgm_list;
		while (new_ptr != NULL)
		{
			new_prg = new_ptr->content;
			if (ft_strequ(orig_prg->name, new_prg->name) == TRUE)
			{
				// Compare data
				tmp = ft_asprintf("%s no change.\n", orig_prg->name);
				vct_addstr(resp, tmp);
				ft_strdel(&tmp);
				found = 1;
				break ;
			}
			new_ptr = new_ptr->next;
		}
		if (found == 0)
		{
			tmp = ft_asprintf("%s removed.\n", orig_prg->name);
			vct_addstr(resp, tmp);
			ft_strdel(&tmp);
		}
		orig_ptr = orig_ptr->next;
	}

	new_ptr = g_newenv->prgm_list;
	while (new_ptr != NULL) // get removed
	{
		new_prg = new_ptr->content;
		found = 0;
		orig_ptr = g_denv->prgm_list;
		while (orig_ptr != NULL)
		{
			orig_prg = orig_ptr->content;
			if (ft_strequ(orig_prg->name, new_prg->name) == TRUE)
			{
				found = 1;
				break ;
			}
			orig_ptr = orig_ptr->next;
		}
		if (found == 0)
		{
			tmp = ft_asprintf("%s added.\n", new_prg->name);
			vct_addstr(resp, tmp);
			ft_strdel(&tmp);
		}
		new_ptr = new_ptr->next;
	}
	return (resp);
}

t_vector		*reread_file(t_instance *in, t_program *prg)
{
	(void)in;(void)prg;
	dictionary	*dict;
	int			sections;

	dict = iniparser_load(g_denv->opt.str[CONFIGURATION]);
	if (dict == NULL)
		return (vct_newstr("reread: could not open file to read.\n"));
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
	
	reparse_ini_file(dict);

//	print_starting_debug(g_newenv);
	return (register_changes());
}

t_vector			*cmd_reread(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x00)
		vct = reread_file(NULL, NULL);
	return (vct);
}
