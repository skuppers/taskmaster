/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 13:21:56 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/09 20:23:19 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"
#include "iniparser.h"

static void		error_opt(char *msg)
{
	ft_dprintf(STDERR_FILENO,
			"Error: %s\nFor help, use ./taskmasterctl -h\n", msg);
	ft_strdel(&msg);
	exit_routine(NO_MSG);
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
								"-d", "--debug", "-s", "--serverurl",
								"-c", "--configuration"};
	int			count;

	count = 0;
	while (count < NB_OPT)
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
				if (count / 2 == 0)
				{
					if (ft_strnequ(av[i], UNIX_URI, UNIX_URI_SIZE) == FALSE)
						error_opt(ft_asprintf(
						"invalid value for -s '%s' (expected prefix: unix://)",
						av[i], av[i]));
					else
						g_env->opt.str[count / 2] = av[i] + UNIX_URI_SIZE;
						
				}
				else
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

void		get_opt(int ac, char **av)
{
	int			i;
	int			sections;
	char		*tmp;

	i = 0;
	g_env->opt.str[CONFIGURATION] = DFL_CONFIGURATION;
	g_env->opt.str[PROMPT] = DFL_PROMPT;
	g_env->opt.str[SERVERURL] = DFL_URL;
	while (i < ac && av[i][0] == '-' && ft_strequ(av[i], "--") == FALSE)
		i += parse_opt(av, ac, i);
	set_shell_mode(ac, av, i);
	if (g_env->opt.mask & OPT_HELP)
		print_help();
	g_env->dict = parse_inifile(g_env->opt.str[CONFIGURATION]);
	sections = iniparser_getnsec(g_env->dict);
	while (sections >= 0)
	{
		if (ft_strequ(iniparser_getsecname(g_env->dict, sections),
					"taskmasterctl") == 1)
		{
			tmp = (char *)iniparser_getstring(g_env->dict,
						"taskmasterctl:serverurl", NULL);
			if ((g_env->opt.mask & OPT_SERVERURL) == FALSE && tmp != NULL)
				g_env->opt.str[SERVERURL] = tmp;
			tmp = (char *)iniparser_getstring(g_env->dict,
						"taskmasterctl:prompt", NULL);
			if (tmp != NULL && ft_strlen(tmp) != 0)
				g_env->opt.str[PROMPT] = tmp;
			break ;
		}
		--sections;
	}
	
}

void	check_opt(void)
{
	if (g_env->opt.str[SERVERURL] == NULL)
	{
		dprintf(2, "taskmasterctl:serverurl is undefined! "
					"Check your taskmaster.conf file.\n\n");
		exit_routine(NO_MSG);
	}
}
