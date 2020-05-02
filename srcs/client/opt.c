/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 13:21:56 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/01 16:56:03 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"
#include "iniparser.h"

static void		error_opt(char *msg)
{
	ft_dprintf(STDERR_FILENO,
			"Error: %s\nFor help, use ./taskmasterctl -h\n", msg);
	ft_strdel(&msg);
	exit_routine();
}

static void		set_shell_mode(int ac, char **av, int i)
{
	if (i != ac)
	{
		g_env->opt.mask |= OPT_BATCHCMD;
		g_env->opt.batch_cmd = vct_new(0);
		while (i < ac)
		{
			vct_addstr(g_env->opt.batch_cmd, av[i]);
			i++;
			if (i != ac)
				vct_add(g_env->opt.batch_cmd, ' ');
		}
	}
	else
		g_env->opt.mask |= OPT_INTERACTIVE;
}

static int		parse_opt(char **av, int ac, int i)
{
	const char	*opt_str[] = {"-h", "--help", "-i", "--interactive",
				"-d", "--debug", "-s", "--serverurl", "-u", "--username", "-p",
				 "--password", "-c", "--configuration"};
	int			count;

	count = 0;
	while (count < 14)
	{
		if (ft_strequ(av[i], opt_str[count]) == TRUE)
		{
			g_env->opt.mask |= (1 << (count / 2));
			if (count >= 6)
			{
				i++;
				if (i == ac || av[i][0] == '-')
					error_opt(ft_asprintf("option '%s' requires argument",
							opt_str[count]));
				count -= 6;
				if (count / 2 == 0
						&& ft_strnequ(av[i], "unix://", 7) == FALSE)
					error_opt(ft_asprintf(
							"invalid value for -s '%s': value '%s' is not a URL",
							av[i], av[i]));
				g_env->opt.str[count / 2] = av[i];
				return (2);
			}
			return (1);
		}
		count++;
	}
	error_opt(ft_asprintf("option '%s' not recognized", av[i]));
	return (0);
}

void		get_opt(t_env *env, int ac, char **av)
{
	int			i;
	int			sections;
	char		*tmp;

	i = 0;
	env->opt.str[CONFIGURATION] = DFL_CONFIGURATION;
	env->opt.str[PROMPT] = DFL_PROMPT;
	env->opt.str[SERVERURL] = DFL_URL;
	while (i < ac && av[i][0] == '-' && ft_strequ(av[i], "--") == FALSE)
		i += parse_opt(av, ac, i);
	set_shell_mode(ac, av, i);
	if (env->opt.mask & OPT_HELP)
		print_help();
	env->dict = parse_inifile(env->opt.str[CONFIGURATION]);
	sections = iniparser_getnsec(env->dict);
	while (sections >= 0)
	{
		if (ft_strequ(iniparser_getsecname(env->dict, sections), "taskmasterctl") == 1)
		{
			tmp = (char *)iniparser_getstring(env->dict, "taskmasterctl:username", NULL);
			if ((env->opt.mask & OPT_USERNAME) == FALSE && tmp != NULL)
				env->opt.str[USERNAME] = tmp;
			tmp = (char *)iniparser_getstring(env->dict, "taskmasterctl:password", NULL);
			if ((env->opt.mask & OPT_PASSWORD) == FALSE && tmp != NULL)
				env->opt.str[PASSWORD] = tmp;
			tmp = (char *)iniparser_getstring(env->dict, "taskmasterctl:serverurl", NULL);
			if ((env->opt.mask & OPT_SERVERURL) == FALSE && tmp != NULL)
				env->opt.str[SERVERURL] = tmp;
			tmp = (char *)iniparser_getstring(env->dict, "taskmasterctl:prompt", NULL);
			if (tmp != NULL && ft_strlen(tmp) != 0)
				env->opt.str[PROMPT] = tmp;
			return ;
		}
		--sections;
	}
	
}

int8_t	check_opt(t_env *env)
{
	if (env->opt.str[SERVERURL] == NULL)
	{
		dprintf(2, "taskmasterctl:serverurl is undefined! Check your taskmaster.conf file.\n\n");
		exit_routine();
	}
	return (0);
}
