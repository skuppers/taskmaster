/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt_d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 13:21:56 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 18:35:15 by ffoissey         ###   ########.fr       */
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

static void		get_arg_opt(char *arg, const int count, const char *opt_str[])
{
	if (arg == NULL || arg[0] == '-')
		error_opt(ft_asprintf("option '%s' requires argument", opt_str[count]));
	g_denv->opt.str[count] = arg;
}

static int		get_one_opt(char **av, int i)
{
	const char	*opt_str[] = {"-h", "--help", "-n", "--nodaemon",
							"-v", "--version", "-c", "--configuration",
							"-u", "--user", "-d", "--directory",
							"-l", "--logfile", "-e", "--loglevel",
							"-q", "--childlogdir", "-a", "--minfds",
							"-p","--minprocs"};
	int			count;

	count = 0;
	while (count < NB_OPT)
	{
		if (ft_strequ(av[i], opt_str[count]) == true)
		{
			g_denv->opt.optmask |= (1 << (count / 2));
			if (count < OPT_WITHOUT_ARG)
				return (1);
			get_arg_opt(av[i + 1], (count - OPT_WITHOUT_ARG) / 2, opt_str);
			return (2);
		}
		count++;
	}
	error_opt(ft_asprintf("option '%s' not recognized", av[i]));
	return (0);
}

static void		parse_opt(int ac, char **av)
{
	int			i;

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
				i += get_one_opt(av, i);
			continue ;
		}
		error_opt(ft_asprintf("option '%s' not recognized", av[i]));
	}
}

void			get_opt(int ac, char **av)
{
	int			sections;

	parse_opt(ac, av);
	if ((g_denv->opt.optmask & OPT_HELP) || (g_denv->opt.optmask & OPT_VERSION))
	{
		g_denv->opt.optmask & OPT_HELP ? print_help() : print_version();
		exit_routine(NO_MSG);
	}
	g_denv->dict = load_ini_file(g_denv->opt.str[CONFIGURATION]);
	sections = iniparser_getnsec(g_denv->dict);
	while (sections >= 0)
	{
		if (ft_strequ(iniparser_getsecname(g_denv->dict, sections),
					"taskmasterd") == true)
		{
			taskmasterd_override(g_denv, g_denv->dict);
			return ;
		}
		--sections;
	}
	if (sections < 0)
		dprintf(STDERR_FILENO, "taskmasterd: [taskmasterd]"
								" section is not defined.\n");
}
