/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dtransfert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/16 12:11:37 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void	handle_client_data(t_vector *vct, const int fd)
{
	t_cmd			*cmd;
	t_vector		*resp;

	if (ft_strequ(g_denv->opt.str[LOGLEVEL], LOGLVL_DEBG) == true
			&& (g_denv->opt.optmask & OPT_NODAEMON))
		debug_print_bytecode(vct);
	cmd = decode_cmd(vct);
	if (cmd == NULL)
		tlog(E_LOGLVL_ERRO, "Error: Bad trame\n");
	else
	{
		resp = execute_cmd(cmd);
		if (resp != NULL)
		{
			tlog(E_LOGLVL_DEBG, "Responding.\n");
			if (ft_strequ(g_denv->opt.str[LOGLEVEL], LOGLVL_DEBG) == true
					&& (g_denv->opt.optmask & OPT_NODAEMON))
				debug_print_bytecode(resp);
			try_to_send_trame(fd, resp, TO_LOG, tlog);
		}
		vct_del(&resp);
		ft_free_tab_str(cmd->av);
		if (g_denv->shutdown != FALSE)
			exit_routine(NO_MSG);
	}
	vct_creadline(NULL, CLEANUP, 0);
	fdatasync(fd);
}

void	handle_client_requests(const int fd_nb, fd_set *master_set)
{
	int32_t		readstatus;
	t_vector	*vct;

	vct = vct_new(DFL_VCT_SIZE);
	if ((readstatus = vct_creadline(vct, fd_nb, EOT)) <= 0)
	{
		if (readstatus == FAILURE)
			tlog(E_LOGLVL_ERRO, "Vct_readline() failed: %s\n", strerror(errno));
		else
			tlog(E_LOGLVL_DEBG, "=> Client disconnected\n");
		close(fd_nb);
		FD_CLR(fd_nb, master_set);
		g_denv->client_connected = 0;
	}
	else
		handle_client_data(vct, fd_nb);
	vct_del(&vct);
}
