/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dtransfert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/06 16:52:12 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"


void	handle_client_data(t_denv *env, t_vector *vct, int32_t readstatus, int fd)
{
	t_cmd			*cmd;
	t_vector		*resp;

	dprintf(STDERR_FILENO, "------------------------------------------\n");
	dprintf(STDERR_FILENO, "read %u bytes | trame len: %zu bytes\n",
				readstatus, vct_len(vct));
	
	if (ft_strequ(env->opt.str[LOGLEVEL], "debug") == 1)
			debug_print_bytecode(vct); // DEBUG
	
	cmd = decode_cmd(vct);
	if (cmd == NULL)
		tlog(E_LOGLVL_ERRO, "Error: Bad trame\n");
	else
	{
		if (ft_strequ(env->opt.str[LOGLEVEL], "debug") == 1)
			debug_cmd(cmd); // DEBUG
		resp = execute_cmd(cmd);
		if (resp != NULL)
		{
			tlog(E_LOGLVL_DEBG, "Responding.\n");
			debug_print_bytecode(resp); // DEBUG
			try_to_send_trame(fd, resp, TO_LOG, tlog);
		}
		vct_del(&resp);
		ft_free_tab_str(cmd->av);
	}
	dprintf(STDERR_FILENO, "------------------------------------------\n");
	if (env->shutdown == TRUE)
		exit_routine();
}

void	realine_error(t_denv *env, int32_t readstatus, int fd_nb, fd_set *master_set)
{
	if (readstatus == -1)
	  	tlog(E_LOGLVL_ERRO, "Vct_readline() failed\n");
	else if (readstatus == 0)
	 	tlog(E_LOGLVL_DEBG, "=> Client disconnected\n");
	close(fd_nb);
	FD_CLR(fd_nb, master_set);
	env->client_connected = 0;
}

void	handle_client_requests(t_denv *env, int fd_nb, fd_set *master_set)
{
	int32_t		readstatus;
	t_vector	*vct;

	vct = vct_new(DFL_VCT_SIZE);
	if ((readstatus = vct_creadline(vct, fd_nb, EOT)) <= 0)
		realine_error(env, readstatus, fd_nb, master_set);
	else 
		handle_client_data(env, vct, readstatus, fd_nb);
	vct_del(&vct);
}

void	client_connected(t_denv *env, fd_set *master_set, int connectionfd)
{
	tlog(E_LOGLVL_DEBG, "=> Client connecting...\n");
	if (env->client_connected == 1)
	{
		tlog(E_LOGLVL_DEBG, "=> Client trying to connect, but no slot is avaible\n");
		close(connectionfd);
		return ;
	}
	FD_SET(connectionfd, master_set);
	tlog(E_LOGLVL_DEBG, "=> Client connected\n");
	env->client_connected = 1;
}

void	select_error(t_denv *env)
{
	(void)env;
	tlog(E_LOGLVL_CRIT, "Select syscall failed: %s\n", strerror(errno));
	exit_routine();
}

void	accept_error(t_denv *env)
{
	(void)env;
	tlog(E_LOGLVL_CRIT, "Select accept() failed: %s\n", strerror(errno));
	exit_routine();
}

void	my_select(t_denv *env, fd_set *recv_set)
{
	struct timeval	tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;
	if (select(DFL_FD_SETSIZE, recv_set, NULL, NULL, &tv) < 0)
		select_error(env);
}

void listen_for_data(t_denv *env)
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
