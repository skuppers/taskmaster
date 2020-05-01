/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 19:05:55 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

t_env	*g_env;

void	check_working_directory(void)
{
	DIR* dir;
	
	dir = opendir(DFL_WORKDIR);
	if (dir)
		closedir(dir);
	else if (ENOENT == errno)
	{
		if (mkdir(DFL_WORKDIR, 0744) == -1)
		{
			taskmaster_fatal("check_working_directory(): Could not create working directory", strerror(errno));
			exit_routine();
		}
	}
	else
	{
    	taskmaster_fatal("check_working_directory(): Could not open working directory", strerror(errno));
		exit_routine();
	}
}

int main(int ac, char **av)
{
	(void)ac;(void)av;
	t_env   env;

	ft_memset(&env, 0, sizeof(t_env));
	ft_memset(&env.opt, 0, sizeof(t_options));
	g_env = &env;

	set_taskmasterd_defautls(&env);
	//do cmdline opt parsing here
	
	// check working dir
	check_working_directory();

	// load ini file
	env.dict = load_ini_file(env.opt.configfile);
	if (env.dict == NULL)
		exit_routine();

	// parse ini file
	parse_ini_file(&env, env.dict);


	//init logger
	if (init_log(&env) != 0)
		return (-1);


	// Do config file parsing

	// make network things
	if (make_socket(&env, DFL_SOCKET) != 0)
		return (-1);
	if (bind_socket(&env) != 0)
		return (-1);

	//once all is ready, daemonize
	// And listen for incoming connections

	listen_for_data(&env);
	
	close(env.unix_socket);
	unlink(DFL_SOCKET);

	return (0);
}
