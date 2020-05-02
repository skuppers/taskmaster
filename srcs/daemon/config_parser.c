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
	iniparser_dump(ini_dict, stdout);
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
}

int8_t	append_to_grplist(t_env *env, t_group *grp)
{
	t_list		*list;

	list = ft_lstnew(grp, sizeof(t_group));
	if (env->goup_list == NULL)
		env->goup_list = list;
	else
		ft_lstadd(&env->goup_list, list);
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
		secname = iniparser_getsecname(env->dict, sections);
		print_log(env, E_LOGLVL_DEBG, "Inifile: parsing section: %s\n", secname);
		if (ft_strnequ(secname, "program.", 8) == 1)
		{
			prog.name = ft_strsub(secname, 8, ft_strlen(secname) - 8);
			print_log(env, E_LOGLVL_DEBG, "Inifile: found program: %s\n", prog.name);
		}
		else if (ft_strnequ(secname, "group.", 6) == 1)
		{
			group.name = ft_strsub(secname, 6, ft_strlen(secname) - 6);
			print_log(env, E_LOGLVL_DEBG, "Inifile: found group: %s\n", group.name);
		}
		else if (ft_strequ(secname, "taskmasterd") == 0
					&& ft_strequ(secname, "taskmasterctl") == 0)
		{
			print_log(env, E_LOGLVL_ERRO, "Inifile: Unknown section: %s\n", secname);
		}
	}
}
