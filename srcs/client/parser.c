/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/01 15:29:40 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static void	debug_print_bytecode(t_vector *bytecode)
{
	size_t	i;
	char	c;

	i = 0;
	ft_putendl_fd("\n ----- Reminder Table ----- \033[35m", STDERR_FILENO);
	ft_putendl_fd("Start of Header (SOH) = 0x01", STDERR_FILENO);
	ft_putendl_fd("Start Of Text   (STX) = 0x02", STDERR_FILENO);
	ft_putendl_fd("End Of Text     (ETX) = 0x03", STDERR_FILENO);
	ft_putendl_fd("End Of Line     (ENQ) = 0x05", STDERR_FILENO);
	ft_putendl_fd("Unit Separator  (US)  = 0x1f", STDERR_FILENO);
	ft_putendl_fd("\033[0m -------------------------- ", STDERR_FILENO);
	ft_putstr_fd("\033[31mBytecode: \033[32m", STDERR_FILENO);
	while (i + 1 < vct_len(bytecode))
	{
		c = vct_getcharat(bytecode, i);
		if (i == 1)
		{
			ft_dprintf(STDERR_FILENO, "[size = %u]",
					(*((uint64_t *)(vct_getstr(bytecode))) >> 8) & 0xffffffff);
			i += 3;
		}
		else if (ft_isprint(c) == TRUE)
			ft_putchar_fd(c, STDERR_FILENO);
		else if (c == US || c == STX || c == ETX)
			ft_dprintf(STDERR_FILENO, "\033[34m[0x%.2hhx]\033[32m", c);
		else if (c == SOH || c == ENQ)
			ft_dprintf(STDERR_FILENO, "\033[35m[0x%.2hhx]\033[32m", c);
		else
			ft_dprintf(STDERR_FILENO, "\033[36m[0x%.2hhx]\033[32m", c);
		i++;
	}
	ft_dprintf(STDERR_FILENO, "\033[0m\n");
}

const char *get_keyword(const uint8_t i)
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

void	debug_cmd(t_cmd *cmd)
{
	ft_printf("cmd [%d] (%s) | ac = %d\n", cmd->type, get_keyword(cmd->type),
			cmd->ac);
	for (int i = 0; i < cmd->ac; i++)
		ft_printf("ARG[%d] = `%s'\n", i, cmd->av[i]);
}

int	parser(t_vector *line)
{
	t_vector			*cmd_string;
	t_vector			*bytecode;
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
		ft_dprintf(STDERR_FILENO, "*** Unknown syntax: %s %s\n",
					vct_getstr(cmd_string), vct_getstr(line));
		vct_del(&cmd_string);
		return (FAILURE);
	}
	vct_trimfront(line, " ");
	g_env->cmd = get_cmd_struct(cmd_type, line);
//	debug_cmd(g_env->cmd); // DEBUG
	bytecode = builtin[cmd_type](g_env->cmd);
	if (bytecode != NULL)
	{
		if (g_env->opt.mask & OPT_DEBUG)
			debug_print_bytecode(bytecode);	
			///////////////// LAUNCH CMD
		send_bytecode(bytecode, (uint16_t)vct_len(bytecode));
	}
	ft_free_tab_str(g_env->cmd->av);
	g_env->cmd->av = NULL;
	vct_del(&bytecode);
	vct_del(&cmd_string);
	return (SUCCESS);
}
