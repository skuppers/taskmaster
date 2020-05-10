/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 11:42:22 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 13:44:59 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

static void	print_other_char(const char c)
{
	if (c == '\t' || c == '\n')
		ft_putstr_fd(c == '\t' ? "\\t" : "\\n", STDERR_FILENO);
	else if (c == US)
		ft_putstr_fd("\033[34m[US]\033[32m", STDERR_FILENO);
	else if (c == STX)
		ft_putstr_fd("\033[34m[STX]\033[32m", STDERR_FILENO);
	else if (c == ETX)
		ft_putstr_fd("\033[34m[ETX]\033[32m", STDERR_FILENO);
	else if (c == SOH)
		ft_putstr_fd("\033[35m[SOH]\033[32m", STDERR_FILENO);
	else if (c == ENQ)
		ft_putstr_fd("\033[35m[ENQ]\033[32m", STDERR_FILENO);
	else if (c == EOT)
		ft_putstr_fd("\033[36m[EOT]\033[32m", STDERR_FILENO);
	else
		ft_dprintf(STDERR_FILENO, "\033[30m[0x%.2hhx]\033[32m", c);
}

void		debug_print_bytecode(t_vector *bytecode)
{
	size_t		i;
	char		c;

	i = 0;
	ft_putstr_fd("\033[31;1mBytecode: \033[32m", STDERR_FILENO);
	while (i < vct_len(bytecode))
	{
		c = vct_getcharat(bytecode, i);
		if (i == 1)
		{
			ft_dprintf(STDERR_FILENO, "\033[33m[size = %u]\033[32m",
					(*((uint64_t *)(vct_getstr(bytecode))) >> 8) & 0xffffffff);
			i += 3;
		}
		else if (ft_isprint(c) == TRUE)
			ft_putchar_fd(c, STDERR_FILENO);
		else
			print_other_char(c);
		i++;
	}
	ft_dprintf(STDERR_FILENO, "\033[0;0m\n");
}


const char *get_keyword(const uint8_t i)
{
	static const char	*grammar[] = {"add", "avail", "clear", "exit", "fg",
								"help", "maintail", "open", "pid", "quit",
								"reload", "remove", "reread", "restart",
								"shutdown", "signal", "start", "status", "stop",
								"tail", "update", "version", "syn", "ack"};

	return (grammar[i]);
}

void		debug_cmd(t_cmd *cmd)
{
	int		i;

	i = 0;
	dprintf(STDERR_FILENO, "cmd [%#.2x] (%s) | ocp = %#.2x | ac = %d\n",
				cmd->type + 128, get_keyword(cmd->type), cmd->ocp, cmd->ac);
	while (i < cmd->ac)
	{
		dprintf(STDERR_FILENO, "ARG[%d] = `%s'\n", i, cmd->av[i]);
		i++;
	}
}
