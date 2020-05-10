/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 10:54:25 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 11:26:53 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"
#include "iniparser.h"

static dictionary	*parse_inifile(char *str)
{
	dictionary	*ini_dict;

	ini_dict = iniparser_load(str);
	if (ini_dict == NULL)
	{
		dprintf(STDERR_FILENO, "ERROR: Could not read ini file\n");
		exit_routine(ERR, strerror(errno));
	}
	return (ini_dict);
}

static int			get_taskmasterctl_section_nb(void)
{
	int			section;
	const char	*str_section;

	section = iniparser_getnsec(g_env->dict);
	while (section >= 0)
	{
		str_section = iniparser_getsecname(g_env->dict, section);
		if (ft_strequ(str_section, "taskmasterctl") == true)
			break ;
		--section;
	}
	return (section);
}

void				load_config(void)
{
	char		*tmp;
	int			section;

	g_env->dict = parse_inifile(g_env->opt.str[CONFIGURATION]);
	section = get_taskmasterctl_section_nb();
	if (section >= 0)
	{
		tmp = (char *)iniparser_getstring(g_env->dict, SERVERURL_SEC, NULL);
		if ((g_env->opt.mask & OPT_SERVERURL) == false && tmp != NULL)
			g_env->opt.str[SERVERURL] = tmp;
		if (g_env->opt.str[SERVERURL] == NULL)
			exit_routine(ERR, "taskmasterctl:serverurl is undefined! "
						"Check your taskmaster.conf file.\n\n");
		tmp = (char *)iniparser_getstring(g_env->dict, PROMPT_SEC, NULL);
		if (tmp != NULL && ft_strlen(tmp) != 0)
			g_env->opt.str[PROMPT] = tmp;
	}
}
