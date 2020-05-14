/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_reread.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:32:52 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 17:36:46 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

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

t_vector		*check_taskmasterd_section(dictionary *dict)
{
	int			sections;

	sections = iniparser_getnsec(dict);
	while (sections >= 0)
	{
		if (ft_strequ(iniparser_getsecname(dict, sections),
				"taskmasterd") == true)
		{
			if (check_daemon_opts(dict) == FAILURE)
			{
				return (vct_newstr("reread: You cannot change the"
				" taskmasterd section at runtime.\n"));
			}
			return (NULL);
		}
		--sections;
	}
	return (vct_newstr("[taskmasterd] section is not defined.\n"));
}

t_vector		*reread_file(t_instance *in, t_program *prg)
{
	dictionary	*dict;
	t_denv		*tmpenv;
	t_vector	*vct;

	(void)in;
	(void)prg;
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
	vct = check_taskmasterd_section(dict);
	if (vct != NULL)
		return (vct);
	reparse_ini_file(dict, tmpenv);
	return (register_changes(tmpenv));
}

t_vector		*cmd_reread(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x00)
		vct = reread_file(NULL, NULL);
	return (vct);
}
