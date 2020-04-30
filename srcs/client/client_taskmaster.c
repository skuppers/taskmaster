/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/30 12:07:39 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_env		*g_env;

static int	print_prompt(void)
{
	return (ft_dprintf(STDERR_FILENO, PROMPT));
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

int		main(void)
{
	t_env	environment;

	if (isatty(STDIN_FILENO) == FALSE)
	{
		ft_dprintf(STDERR_FILENO, "Not a tty\n");
		return (EXIT_FAILURE);
	}

	g_env = &environment;
	ft_memset(&environment, 0, sizeof(environment));

	connect_to_daemon(&environment, DFLT_SOCKET);
	
	init_readline(&environment);

	init_signals();

	read_cmd(&environment);

	exit_routine();
	return (EXIT_SUCCESS);
}
