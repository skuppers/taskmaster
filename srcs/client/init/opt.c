/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 13:21:56 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 12:44:17 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static void		error_opt(char *msg)
{
	ft_dprintf(STDERR_FILENO,
			"Error: %s\nFor help, use ./taskmasterctl -h\n", msg);
	ft_strdel(&msg);
	exit_routine(NO_MSG);
}

static void		set_shell_mode(char **av, int i)
{
	if (av[i] == NULL)
	{
		g_env->opt.mask |= OPT_INTERACTIVE;
		return ;
	}
	g_env->opt.mask |= OPT_BATCHCMD;
	g_env->opt.batch_cmd = vct_new(0);
	while (av[i] != NULL)
	{
		vct_addstr(g_env->opt.batch_cmd, av[i]);
		i++;
		if (av[i] != NULL)
			vct_add(g_env->opt.batch_cmd, ' ');
	}
}

static void		get_config_opt(const char *opt_str[],
					char *arg, const int count)
{
	if (arg == NULL || arg[0] == '-')
		error_opt(ft_asprintf("option '%s' requires argument",
				opt_str[count]));
	if (count == 0)
	{
		if (ft_strnequ(arg, UNIX_URI, UNIX_URI_SIZE) == true)
			g_env->opt.str[count] = arg + UNIX_URI_SIZE;
		else
			error_opt(ft_asprintf(
						"invalid value for -s '%s' (expected prefix: unix://)",
						arg));
	}
	else
		g_env->opt.str[count] = arg;
}

static int		parse_opt(char **av, const int i)
{
	const char	*opt_str[] = {"-h", "--help", "-i", "--interactive",
								"-d", "--debug", "-s", "--serverurl",
								"-c", "--configuration"};
	int			count;

	count = 0;
	while (count < NB_OPT)
	{
		if (ft_strequ(av[i], opt_str[count]) == true)
		{
			g_env->opt.mask |= (1 << (count / 2));
			if (count >= NB_NOCONF_OPT)
			{
				get_config_opt(opt_str, av[i + 1], (count - NB_NOCONF_OPT) / 2);
				return (2);
			}
			return (1);
		}
		count++;
	}
	error_opt(ft_asprintf("option '%s' not recognized", av[i]));
	return (0);
}

void			get_opt(const int ac, char **av)
{
	int			i;

	i = 0;
	g_env->opt.str[CONFIGURATION] = DFL_CONFIGURATION;
	g_env->opt.str[PROMPT] = DFL_PROMPT;
	g_env->opt.str[SERVERURL] = DFL_URL;
	while (i < ac && av[i][0] == '-' && ft_strequ(av[i], "--") == false)
		i += parse_opt(av, i);
	set_shell_mode(av, i);
	if (g_env->opt.mask & OPT_HELP)
	{
		print_help();
		exit_routine(NO_MSG);
	}
	load_config();
}
