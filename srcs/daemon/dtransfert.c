/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dtransfert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 19:46:16 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

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

void	handle_client_data(t_env *env, t_vector *vct, int32_t readstatus)
{
	t_cmd			*cmd;

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
		execute_cmd(cmd);
		ft_free_tab_str(cmd->av);
	}
	ft_printf("------------------------------------------\n");
}

void	realine_error(t_env *env, int32_t readstatus, int fd_nb, fd_set *master_set)
{
	if (readstatus == -1)
	  	print_log(env, E_LOGLVL_ERRO, "Vct_readline() failed\n");
	else if (readstatus == 0)
	 	print_log(env, E_LOGLVL_DEBG, "=> Client disconnected\n");
	close(fd_nb);
	FD_CLR(fd_nb, master_set);
	env->client_connected = 0;
}

void	handle_client_requests(t_env *env, int fd_nb, fd_set *master_set)
{
	int32_t		readstatus;
	t_vector	*vct;

	vct = vct_new(DFL_VCT_SIZE);
	if ((readstatus = vct_creadline(vct, fd_nb, EOT)) <= 0)
		realine_error(env, readstatus, fd_nb, master_set);
	else 
		handle_client_data(env, vct, readstatus);
	vct_del(&vct);
}

void	client_connected(t_env *env, fd_set *master_set, int connectionfd)
{
	print_log(env, E_LOGLVL_DEBG, "=> Client connecting...\n");
	if (env->client_connected == 1)
	{
		print_log(env, E_LOGLVL_DEBG, "=> Client trying to connect, but no slot is avaible\n");
		close(connectionfd);
		return ;
	}
	FD_SET(connectionfd, master_set);
	print_log(env, E_LOGLVL_DEBG, "=> Client connected\n");
	env->client_connected = 1;
}

void	select_error(t_env *env)
{
	taskmaster_fatal("select()", strerror(errno));
	print_log(env, E_LOGLVL_CRIT, "Select syscall failed: %s\n", strerror(errno));
	exit_routine();
}

void	accept_error(t_env *env)
{
	taskmaster_fatal("accept()", strerror(errno));
	print_log(env, E_LOGLVL_CRIT, "Select accept failed: %s\n", strerror(errno));
	exit_routine();
}

void	my_select(t_env *env, fd_set *recv_set)
{
	struct timeval	tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;
	if (select(DFL_FD_SETSIZE, recv_set, NULL, NULL, &tv) < 0)
		select_error(env);
}

void listen_for_data(t_env *env)
{
    int		 		connectionfd;
	fd_set			recv_set;
	fd_set			master_set;
	int				fd_nb;

	FD_ZERO (&master_set);
	FD_ZERO (&recv_set);
	FD_SET (env->unix_socket, &master_set);
	while (1)
	{
		recv_set = master_set;
		my_select(env, &recv_set);
		fd_nb = -1;
		while (++fd_nb < DFL_FD_SETSIZE)
		{
			if (FD_ISSET(fd_nb, &recv_set) == 0)
				continue ;
			if (fd_nb == env->unix_socket)
			{
				if ((connectionfd = accept(env->unix_socket, NULL, NULL)) == -1)
		 				accept_error(env);
				else
					client_connected(env, &master_set, connectionfd);
			}
			else
				handle_client_requests(env, fd_nb, &master_set);
		}
		waiter(env);
	}
}
