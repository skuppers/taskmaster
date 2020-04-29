/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 16:19:21 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static const char *get_keyword(const uint8_t i)
{
	static const char	*grammar[] = {"add", "avail", "clear", "exit", "fg",
								"maintail", "open", "pid", "quit", "reload",
								"remove", "reread", "restart", "shutdown",
								"signal", "start", "status", "stop", "tail",
								"update", "version"};

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

static int	parser(t_vector *line)
{
	t_vector			*cmd_string;
	t_cmd				*cmd;
	enum e_cmd_type		cmd_type;
	static	t_builtin	builtin[] = {blt_add, blt_avail, blt_clear, blt_exit,
									blt_fg, blt_maintail, blt_open, blt_pid,
									blt_quit, blt_reload, blt_remove,
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
	builtin[cmd_type](line);
	debug_cmd(cmd);
		///////////////// LAUNCH CMD
	ft_free_tab_str(cmd->av);
	return (SUCCESS);
}

static int	print_prompt(void)
{
	return (ft_dprintf(STDERR_FILENO, PROMPT));
}

static void read_cmd(void)
{
	t_vector	*line;
	int			ret;

	line = vct_new(DFL_VCT_SIZE);
	print_prompt();
	while ((ret = vct_readline(line, STDIN_FILENO)) > 0)
	{
		if (vct_apply(line, IS_SPACE) == FALSE)
		{
			parser(line);
			ft_putchar('\n'); // DEBUG
		}
		print_prompt();
	}
	vct_del(&line);
	//if (ret == FAILURE) read_error
}

int		main(void)
{
	if (isatty(STDIN_FILENO) == FALSE)
	{
		ft_dprintf(STDERR_FILENO, "Not a tty\n");
		return (EXIT_FAILURE);
	}
	read_cmd();
	return (EXIT_SUCCESS);
}
