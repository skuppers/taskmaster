/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 19:05:55 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

t_env		*g_env;

static const char *get_keyword(const uint8_t i)
{
	static const char	*grammar[] = {"add", "avail", "clear", "exit", "fg",
								"help", "maintail", "open", "pid", "quit",
								"reload", "remove", "reread", "restart",
								"shutdown", "signal", "start", "status", "stop",
								"tail", "update", "version"};

	return (grammar[i]);
}

static enum e_cmd_type	get_cmd(t_vector *word)
{
	uint8_t	i;

	i = 0;
	while (i < NB_CMD)
	{
		if (ft_strequ(vct_getstr(word), get_keyword(i)) == TRUE)
			return ((enum e_cmd_type)i);
		i++;
	}
	return (BAD_CMD);
}

static t_vector	*clean_line(t_vector *line)
{
	vct_replacechar(line, '\n', ' ');
	vct_replacechar(line, '\t', ' ');
	vct_trimfront(line, " ");
	return (line);
}

static void	debug_cmd(t_cmd *cmd)
{
	ft_printf("cmd [%d] (%s) | ac = %d\n", cmd->type, get_keyword(cmd->type), cmd->ac);
	for (int i = 0; i < cmd->ac; i++)
		ft_printf("ARG[%d] = `%s'\n", i, cmd->av[i]);
}

static void	debug_print_bytecode(t_vector *bytecode)
{
	size_t	i;
	char	c;

	i = 0;
	ft_printf("\033[31mBytecode: \033[32m");
	while (i < vct_len(bytecode))
	{
		c = vct_getcharat(bytecode, i);
		if (ft_isprint(c) == TRUE)
			ft_putchar(c);
		else
			ft_printf("\033[34m0x%.2hhx\033[32m", c);
		i++;
	}
	ft_printf("\033[0m");
}

static int	parser(t_vector *line)
{
	t_vector			*cmd_string;
	t_vector			*bytecode;
	t_cmd				*cmd;
	enum e_cmd_type		cmd_type;
	static	t_builtin	builtin[] = {blt_add, blt_avail, blt_clear, blt_exit,
									blt_fg, blt_help, blt_maintail, blt_open,
									blt_pid, blt_quit, blt_reload, blt_remove,
									blt_reread, blt_restart, blt_shutdown,
									blt_signal, blt_start, blt_status, blt_stop,
									blt_tail, blt_update, blt_version};

	cmd_string = vct_splitchr(clean_line(line), ' ', DEL_CHAR);
	cmd_type = get_cmd(cmd_string);
	if (cmd_type == BAD_CMD)
	{
		ft_dprintf(STDERR_FILENO, "*** Unknow syntax: %s %s\n",
					vct_getstr(cmd_string), vct_getstr(line));
		return (FAILURE);
	}
	vct_trimfront(line, " ");
	cmd = get_cmd_struct(cmd_type, line);
	debug_cmd(cmd);
	bytecode = builtin[cmd_type](cmd);
	if (bytecode != NULL)
	{
		debug_print_bytecode(bytecode);	
			///////////////// LAUNCH CMD
	}
	ft_free_tab_str(cmd->av);
	vct_del(&bytecode);
	return (SUCCESS);
}

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
	while ((ret = tsk_readline(line, STDIN_FILENO, env)) > 0)
	{
		if (vct_apply(line, IS_SPACE) == FALSE)
		{
			history(line, ADD | RESET);
			parser(line);
			ft_putchar('\n'); // DEBUG
		}
		print_prompt();
	}
	vct_del(&line);
	//if (ret == FAILURE) read_error
}

void	exit_routine(void)
{
	release_termmode(g_env);
	free(g_env->orig);
	free(g_env->taskmst);
	exit(42);
}

int		main(void)
{
	t_env	environment;

	if (isatty(STDIN_FILENO) == FALSE)
	{
		ft_dprintf(STDERR_FILENO, "Not a tty\n");
		return (EXIT_FAILURE);
	}
	
	ft_memset(&environment, 0, sizeof(environment));
	g_env = &environment;
	create_termmode(&environment);
	set_termmode(&environment);
	assign_keycodes(&environment);
	link_keys_functions(environment.actionkeys);

	read_cmd(&environment);

	release_termmode(&environment);
	return (EXIT_SUCCESS);
}