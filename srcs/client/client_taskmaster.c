/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 02:07:13 by ffoissey         ###   ########.fr       */
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
	if (ret == FAILURE)
	{
		perror("ERROR: read: ");
		exit_routine();
	}
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
		"     (default \"unix://%s\").\n"
		"action [arguments] -- see below\n\n"
		"Actions are commands like \"tail\" or \"stop\"."
		"  If -i is specified or no action is\n"
		"specified on the command line, a \"shell\""
		" interpreting actions typed\n"
		"interactively is started.  Use the action "
		"\"help\" to find out about available\nactions.\n", DFL_URL);
	exit_routine();
}


void	get_status()
{
	t_vector			*bytecode;
	t_vector			*response;

	g_env->cmd = get_cmd_struct(STATUS, NULL);
	bytecode = blt_status(g_env->cmd);
	send_bytecode(bytecode, vct_len(bytecode));
	response = get_response(g_env);
	if (response == NULL)
		dprintf(STDERR_FILENO, "Got no feedback from daemon!\n");
	else
		vct_print_fd(response, STDERR_FILENO);
	ft_free_tab_str(g_env->cmd->av);
	g_env->cmd->av = NULL;
	vct_del(&bytecode);
	vct_del(&response);
}


int		main(int ac, char **av)
{
	t_env	environment;

	ft_bzero(&environment, sizeof(environment));
	g_env = &environment;

	get_opt(&environment, ac - 1, av + 1);
	check_opt(&environment);
	init_signals();
	connect_to_daemon(&environment, environment.opt.str[SERVERURL]);
	
	init_readline(&environment);
	if (environment.opt.mask & OPT_BATCHCMD)
		parser(environment.opt.batch_cmd);
	if ((g_env->opt.mask & OPT_INTERACTIVE) && isatty(STDIN_FILENO) == FALSE)
	{
		ft_dprintf(STDERR_FILENO, "Not a tty\n");
		exit_routine();
	}
	if (environment.opt.mask & OPT_INTERACTIVE)
	{	
		
		get_status();
		read_cmd(&environment);
	}
	exit_routine();
	return (EXIT_SUCCESS);
}
