/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   daemon_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 17:04:18 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

t_denv	*g_denv;

void	print_prog_debug(t_program *prog)
{
	dprintf(STDERR_FILENO, "Program: %s\n", prog->name);
	dprintf(STDERR_FILENO, " - command:\t%s\n", prog->command);
	dprintf(STDERR_FILENO, " - bin:\t%s\n", prog->bin);
	dprintf(STDERR_FILENO, " - numprocs:\t%d\n", prog->numprocs);
	dprintf(STDERR_FILENO, " - autostart:\t%d\n", prog->autostart);
	dprintf(STDERR_FILENO, " - autorestart:\t%d\n", prog->autorestart);
	dprintf(STDERR_FILENO, " - startsecs:\t%d\n", prog->startsecs);
	dprintf(STDERR_FILENO, " - startretries:\t%d\n", prog->startretries);
	dprintf(STDERR_FILENO, " - stopwaitsec:\t%d\n", prog->stopwaitsecs);
	dprintf(STDERR_FILENO, " - exitcodes:\n");
	ft_print_strtab(prog->exitcodes);
	dprintf(STDERR_FILENO, " - directory:\t%s\n", prog->directory);
	dprintf(STDERR_FILENO, " - umask:\t%u\n", prog->umask);
	dprintf(STDERR_FILENO, " - priority:\t%d\n", prog->priority);
	dprintf(STDERR_FILENO, " - stopsignals:\t%d\n", prog->stopsignal);
	dprintf(STDERR_FILENO, " - user:\t\t%d\n", prog->userid);
	dprintf(STDERR_FILENO, " - stdout_logfile:\t%s\n", prog->stdout_logfile);
	dprintf(STDERR_FILENO, " - stderr_logfile:\t%s\n", prog->stderr_logfile);
	dprintf(STDERR_FILENO, " - environment:\t%s\n\n", prog->environ);
}

void	print_dbg(void)
{
	t_list *ptr;
	
	dprintf(STDERR_FILENO, "Taskmasterd:\n");
	dprintf(STDERR_FILENO, "- logfile: %s\n", g_denv->opt.str[LOGFILE]);
	dprintf(STDERR_FILENO, "- loglevel: %s\n", g_denv->opt.str[LOGLEVEL]);
	dprintf(STDERR_FILENO, "- umask: %o\n", g_denv->opt.umask);
	dprintf(STDERR_FILENO, "- userid: %s\n", g_denv->opt.str[USER]);
	dprintf(STDERR_FILENO, "- directory: %s\n", g_denv->opt.str[DIRECTORY]);
	dprintf(STDERR_FILENO, "- childlogdir: %s\n", g_denv->opt.str[CHILDLOGDIR]);
	dprintf(STDERR_FILENO, "- environment: %s\n\n", g_denv->opt.environ);
	ptr = g_denv->prgm_list;
	while (ptr != NULL)
	{
		print_prog_debug((t_program *)ptr->content);
		ptr = ptr->next;
	}
}

void	set_daemon_environment(char **environ)
{
	g_denv->environ = envtolst(environ);
	if (g_denv->opt.environ != NULL)
	{
		strvalue_to_lst(&g_denv->environ, g_denv->opt.environ);
	//	print_lst(g_denv->environ); // DEBUG LIST ENV
	}
}

void init(int ac, char **av, char **environ)
{
	bzero(g_denv, sizeof(t_denv));
	g_denv->unix_socket = -1;
	set_taskmasterd_defautls(); // DOIT ABSOLUMENT ETRE FAIT EN PREMIER
	check_dflt_directory();
	get_opt(ac - 1, av + 1);
	set_daemon_environment(environ);
	init_log();
	parse_ini_file();
	init_signals();
}

int main(int ac, char **av, char **environ)
{
	t_denv   env;

	g_denv = &env;
	init(ac, av, environ);
	if (ft_strequ(env.opt.str[LOGLEVEL], LOGLVL_DEBG) == true)
		print_dbg();
	daemonize();
	exit_routine(NO_MSG);
	return (EXIT_SUCCESS);
}
