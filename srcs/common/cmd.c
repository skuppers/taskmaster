/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 11:42:22 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/04 21:16:26 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "common.h"

void		debug_print_bytecode(t_vector *bytecode)
{
	size_t		i;
	char		c;

	i = 0;
	ft_putstr_fd("\033[31mBytecode: \033[32m", STDERR_FILENO);
//	ft_putendl_fd("\n ----- Reminder Table ----- \033[35m", STDERR_FILENO);
//	ft_putendl_fd("Start of Header (SOH) = 0x01", STDERR_FILENO);
//	ft_putendl_fd("Start Of Text   (STX) = 0x02", STDERR_FILENO);
//	ft_putendl_fd("End Of Text     (ETX) = 0x03", STDERR_FILENO);
//	ft_putendl_fd("End Of Line     (ENQ) = 0x05", STDERR_FILENO);
//	ft_putendl_fd("Unit Separator  (US)  = 0x1f", STDERR_FILENO);
//	ft_putendl_fd("\033[0m -------------------------- ", STDERR_FILENO);
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
		else if (c == '\t' || c == '\n')
			ft_putstr_fd(c == '\t' ? "\\t" : "\\n", STDERR_FILENO);
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
