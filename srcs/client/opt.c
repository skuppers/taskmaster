/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 13:21:56 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/01 13:51:38 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

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
				"-d", "--debug", "-s", "--serveururl", "-u", "--username", "-p",
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
						&& ft_strnequ(av[i], "http://www", 10) == FALSE)
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

void		get_opt(int ac, char **av)
{
	int		i;

	i = 0;
	g_env->opt.str[S_URL] = "http://localhost:9001";
	while (i < ac && av[i][0] == '-' && ft_strequ(av[i], "--") == FALSE)
		i += parse_opt(av, ac, i);
	set_shell_mode(ac, av, i);
}
