/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt_d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 13:21:56 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/01 16:56:03 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void		error_opt(char *msg)
{
	ft_dprintf(STDERR_FILENO,
			"Error: %s\nFor help, use ./taskmasterd -h\n", msg);
	ft_strdel(&msg);
	exit_routine();
}

static int		parse_opt(char **av, int ac, int i)
{
	const char	*opt_str[] = {"-h", "--help",
								"-n", "--nodaemon",
				                "-v", "--version",
								"-k", "--nocleanup",
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
	while (count < 24)
	{
		if (ft_strequ(av[i], opt_str[count]) == TRUE)
		{
			g_env->opt.optmask |= (1 << (count / 2));
			if (count >= 8) // needs arguments
			{
				i++;
				if (i == ac || av[i][0] == '-')
					error_opt(ft_asprintf("option '%s' requires argument", opt_str[count]));
				count -= 8;
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

	i = 0;
	while (i < ac && av[i][0] == '-' && ft_strequ(av[i], "--") == FALSE)
		i += parse_opt(av, ac, i);

	if (env->opt.optmask & OPT_HELP)
		print_help();
	if (env->opt.optmask & OPT_VERSION)
		print_version();

	env->dict = load_ini_file(env->opt.str[CONFIGURATION]);

	sections = iniparser_getnsec(env->dict);
	while (sections >= 0)
	{
		if (ft_strequ(iniparser_getsecname(env->dict, sections), "taskmasterd") == 1)
		{
			taskmasterd_override(env, env->dict);
			return ;
		}
		--sections;
	}
	if (sections < 0)
	{
		dprintf(STDERR_FILENO, "taskmasterd: [taskmasterd] section is not defined.\n");
	}
}
