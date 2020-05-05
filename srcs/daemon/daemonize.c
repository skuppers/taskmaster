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
	if (env->opt.str[USER] != NULL && setuid(ft_atoi(env->opt.str[USER])) != 0)
	{
		dprintf(STDERR_FILENO, "taskmasterd: cannot change user to UID: %s: %s\n",
			env->opt.str[USER], strerror(errno));
		exit_routine();
	}
	if (env->opt.str[DIRECTORY] != NULL && chdir(env->opt.str[DIRECTORY]) != 0)
	{
		dprintf(STDERR_FILENO, "taskmasterd: cannot chdir to %s: %s\n",
			env->opt.str[DIRECTORY], strerror(errno));
		exit_routine();
	}
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
