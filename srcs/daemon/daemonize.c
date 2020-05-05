/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemonize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 15:29:42 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

int daemonize(t_env *env)
{
	/*
	int fd;

	
	
	pid_t process_id;

	if ((process_id = fork()) < 0)
	{
		dprintf(STDERR_FILENO, "Fatal error: daemonization failed: fork(): %s\n", strerror(errno));
		exit_routine();
	}
	else if (process_id > 0)
	{
		dprintf(STDERR_FILENO, "Process_id of child process %d \n", process_id);
		exit_routine();
	}
//	umask(0);
	if(setsid() < 0)
	{
		dprintf(STDERR_FILENO, "Failed to set new session ID.\n");
		exit_routine();
	}
//	chdir("/");

fd = open("/dev/null", 0);
dup2(fd, STDIN_FILENO);
dup2(fd, STDOUT_FILENO);
dup2(fd, STDERR_FILENO);
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
*/

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
