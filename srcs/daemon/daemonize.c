/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemonize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 21:29:21 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

int daemonize(t_denv *env)
{
	set_uid(env);
	do_chdir(env);
	umask(env->opt.umask);
	daemon(1, 0);
	if (make_socket(env, DFL_SOCKET) != 0)
		exit_routine();
	if (bind_socket(env) != 0)
		exit_routine();
	launch_jobs(env);
	listen_for_data(env);
	exit_routine();
	return (0);
}
