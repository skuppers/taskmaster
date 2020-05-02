/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dtransfert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/01 15:31:28 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

// 32,767 Bytes max message size
int16_t sendall(int sockfd, const char *buf, int16_t buflen)
{
    int16_t     bytes_written;
    int16_t     bytes_left;
    uint8_t     max_retries;

    bytes_written = 0;
    bytes_left = 0;
    max_retries = SEND_PARTIAL_RETRYS;
    while (buflen > 0 && max_retries-- > 0)
    {
        bytes_left = send(sockfd, buf + bytes_written, buflen, 0);
        if (bytes_left == -1)
            return (-1);
        bytes_written += bytes_left;
        buflen -= bytes_written;
    }
    if (max_retries == 0)
        return (-1);
    return (bytes_written);
}

int8_t      send_bytecode(t_vector *code, uint16_t len)
{
	int8_t		status;

	if ((status = sendall(g_env->unix_socket, vct_getstr(code), len)) != 0)
	{
		if (status == -1)
			printf("Fatal error while sending bytecode: %s\n", strerror(errno));
		else if (status == -2)
			printf("Unable to send entire bytecode.\n");
		return (-1);
	}
	printf("Bytecode sent succesfully!\n");
	return (0);
}

static void	debug_print_bytecode(t_vector *bytecode)
{
	size_t		i;
	char		c;

	i = 0;
	ft_putstr_fd("\033[31mBytecode: \033[32m", STDERR_FILENO);
	while (i < vct_len(bytecode))
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

static void	debug_cmd(t_cmd *cmd)
{
	ft_printf("cmd [%#.2x] (%s) | ocp = %#.2x | ac = %d\n",
				cmd->type + 128, get_keyword(cmd->type), cmd->ocp, cmd->ac);
	for (int i = 0; i < cmd->ac; i++)
		ft_printf("ARG[%d] = `%s'\n", i, cmd->av[i]);
}

void listen_for_data(t_env *env)
{
    int		 		connectionfd;
	int 			readstatus;
	t_vector		*vct;
	t_cmd			*cmd;

	vct = vct_new(DFL_VCT_SIZE);
	while (1)
	{
		if ((connectionfd = accept(env->unix_socket, NULL, NULL)) == -1)
		{
			if (errno == EINTR)
				break ;
	  		perror("accept error");
	  		continue;
		}
		printf("Client connected\n");
		while ((readstatus = vct_creadline(vct, connectionfd, EOT)) > 0)
		{
			ft_printf("------------------------------------------\n");
	  		ft_printf("read %u bytes | trame len: %u bytes\n",
				readstatus, vct_len(vct));

			if (ft_strequ(env->opt.str[LOGLEVEL], "debug") == 1)
				debug_print_bytecode(vct); // DEBUG

			cmd = decode_cmd(vct);
			if (cmd == NULL)
				ft_dprintf(STDERR_FILENO, "Error: Bad trame\n");
			else
			{
				if (ft_strequ(env->opt.str[LOGLEVEL], "debug") == 1)
					debug_cmd(cmd); // DEBUG
				ft_free_tab_str(cmd->av);
			}
			ft_printf("------------------------------------------\n");
		}
		if (readstatus == -1)
		{
	  		perror("read");
	  		break;
		}
		else if (readstatus == 0)
		{
	  		printf("Client disconnected\n");
	  		close(connectionfd);
		}
	}
	vct_del(&vct);
}
