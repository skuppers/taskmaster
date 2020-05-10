/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 12:22:45 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/10 12:45:32 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

void	print_help(void)
{
	dprintf(STDERR_FILENO,
		"taskmasterctl -- control applications run"
		" by taskmasterd from the cmd line.\n\n"
		"Usage: ./taskmasterctl [options] [action [arguments]]\n\n"
		"Options:\n"
		"-c/--configuration FILENAME -- configuration file path"
		" (default ./taskmasterd.conf)\n"
		"-h/--help -- print usage message and exit\n"
		"-i/--interactive"
		" -- start an interactive shell after executing commands\n"
		"-s/--serverurl URL -- URL on which taskmaster server is listening\n"
		"     (default \"unix://%s\").\n"
		"action [arguments] -- see below\n\n"
		"Actions are commands like \"tail\" or \"stop\"."
		"  If -i is specified or no action is\n"
		"specified on the command line, a \"shell\""
		" interpreting actions typed\n"
		"interactively is started.  Use the action "
		"\"help\" to find out about available\nactions.\n", DFL_URL);
}
