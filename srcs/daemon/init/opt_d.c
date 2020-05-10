/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt_d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 13:21:56 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 16:59:02 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void		error_opt(char *msg)
{
	ft_dprintf(STDERR_FILENO,
			"Error: %s\nFor help, use ./taskmasterd -h\n", msg);
	ft_strdel(&msg);
	exit_routine(ERR, NULL);
}

static int		parse_opt(char **av, int ac, int i)
{
	const char	*opt_str[] = {"-h", "--help",
								"-n", "--nodaemon",
				                "-v", "--version",
								"-c", "--configuration",
                                "-u", "--user",
                                "-d", "--directory",
                                "-l", "--logfile",
                                "-e", "--loglevel",
								"-q", "--childlogdir",
								"-a", "--minfds",
								"-p","--minprocs"};
	int			count;

	count = 0;
	while (count < NB_OPT)
	{
		if (ft_strequ(av[i], opt_str[count]) == TRUE)
		{
			g_denv->opt.optmask |= (1 << (count / 2));
			if (count >= OPT_WITHOUT_ARG) // needs arguments
			{
				i++;
				if (i == ac || av[i][0] == '-')
					error_opt(ft_asprintf("option '%s' requires argument", opt_str[count]));
				count -= OPT_WITHOUT_ARG;
				g_denv->opt.str[count / 2] = av[i];
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

	i = 0;
	while (i < ac)
	{
		if (av[i][0] == '-')
		{
			if (ft_strequ(av[i], "--") == TRUE)
			{
				if (++i != ac)
					error_opt(ft_asprintf("option '%s' not recognized", av[i]));
			}
			else
				i += parse_opt(av, ac, i);
			continue ;
		}
		error_opt(ft_asprintf("option '%s' not recognized", av[i]));
	}
	if (g_denv->opt.optmask & OPT_HELP)
	{
		print_help();
		exit_routine(NO_MSG);
	}
	if (g_denv->opt.optmask & OPT_VERSION)
	{
		print_version();
		exit_routine(NO_MSG);
	}

	g_denv->dict = load_ini_file(g_denv->opt.str[CONFIGURATION]);

	sections = iniparser_getnsec(g_denv->dict);
	while (sections >= 0)
	{
		if (ft_strequ(iniparser_getsecname(g_denv->dict, sections), "taskmasterd") == 1)
		{
			taskmasterd_override(g_denv, g_denv->dict);
			return ;
		}
		--sections;
	}
	if (sections < 0)
	{
		dprintf(STDERR_FILENO, "taskmasterd: [taskmasterd] section is not defined.\n");
	}
}
