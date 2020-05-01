/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/01 13:27:24 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_env		*g_env;

static int	print_prompt(void)
{
	return (ft_dprintf(STDERR_FILENO, g_env->opt.str[PROMPT]));
}

static void read_cmd(t_env *env)
{
	t_vector	*line;
	int			ret;

	line = vct_new(DFL_VCT_SIZE);
	print_prompt();
	while ((ret = tsk_readline(line, STDIN_FILENO, env)) >= 0)
	{
		if (vct_apply(line, IS_SPACE) == FALSE)
		{
			history(line, ADD | RESET);
			parser(line);
		}
		print_prompt();
	}
	vct_del(&line);
	//if (ret == FAILURE) read_error
}

static void	init_readline(t_env	*environment)
{
	create_termmode(environment);
	set_termmode(environment);
	assign_keycodes(environment);
	link_keys_functions(environment->actionkeys);
}

void	print_help(void)
{
	ft_dprintf(STDERR_FILENO, 
		"taskmasterctl -- control applications run"
		" by taskmasterd from the cmd line.\n\n"
		"Usage: ./taskmasterctl [options] [action [arguments]]\n\n"
		"Options:\n"
		"-c/--configuration FILENAME -- configuration file path"
		" (default ./taskmasterd.conf)\n"
		"-h/--help -- print usage message and exit\n"
		"-i/--interactive"
		" -- start an interactive shell after executing commands\n"
		"-s/--serverurl URL -- URL on which taskmaster server is listening\n"
		"     (default \"%s\").\n"
		"-u/--username USERNAME"
		" -- username to use for authentication with server\n"
		"-p/--password PASSWORD"
		" -- password to use for authentication with server\n\n"
		"action [arguments] -- see below\n\n"
		"Actions are commands like \"tail\" or \"stop\"."
		"  If -i is specified or no action is\n"
		"specified on the command line, a \"shell\""
		" interpreting actions typed\n"
		"interactively is started.  Use the action "
		"\"help\" to find out about available\nactions.\n",
		DFL_URL);
	exit_routine();
}

int		main(int ac, char **av)
{
	t_env	environment;

	if (isatty(STDIN_FILENO) == FALSE)
	{
		ft_dprintf(STDERR_FILENO, "Not a tty\n");
		return (EXIT_FAILURE);
	}
	ft_bzero(&environment, sizeof(environment));
	g_env = &environment;

	get_opt(&environment, ac - 1, av + 1);
	check_opt(&environment);
	
	connect_to_daemon(&environment, environment.opt.str[SERVERURL]);
	
	init_readline(&environment);
	if (environment.opt.mask & OPT_BATCHCMD)
		parser(environment.opt.batch_cmd);
	if (environment.opt.mask & OPT_INTERACTIVE)
	{	
		init_signals();
	
		read_cmd(&environment);
	}
	exit_routine();
	return (EXIT_SUCCESS);
}
