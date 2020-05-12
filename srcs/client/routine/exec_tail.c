/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tail.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 22:45:25 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 23:30:17 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

pid_t		g_pid = 0;

static void	sig_int_hand(int signo)
{
	(void)signo;
	kill(g_pid, SIGINT);
	dprintf(STDERR_FILENO, "^C\n");
}

static char	**get_argtab(uint64_t flag, t_vector *feedback)
{
	char	**arg;

	arg = (char **)malloc(sizeof(char *) * 4);
	if (arg != NULL)
	{
		arg[0] = ft_strdup("tail");
		arg[1] = (flag & TAIL_FIFO) ? ft_strdup("-f") :
									ft_itoa((int)(-((uint32_t)(flag >> 8))));
		vct_cut(feedback);
		arg[2] = vct_dupstr(feedback);
		arg[3] = NULL;
	}
	return (arg);
}

void	exec_tail(uint64_t flag, t_vector *feedback)
{
	char		**arg;
	int			status;
	char		*bin;
	int			fd;

	bin = ft_strdup("tail");
	arg = get_argtab(flag, feedback);
	if (flag & TAIL_FIFO)
		dprintf(STDERR_FILENO, "==> Press CTRL+C to exit <==\n");
	signal(SIGINT, &sig_int_hand);
	if ((g_pid = fork()) == 0)
	{
		if (arg != NULL && (fd = open(arg[2], 0)) != FAILURE
				&& close(fd) == SUCCESS)
			if (get_new_bin_path(&bin, vct_newstr(getenv("PATH"))) == SUCCESS)
				execve(bin, arg, NULL);
		dprintf(STDERR_FILENO, "No log file: %s\n", arg != NULL ? arg[2] : "?");
		exit(EXIT_FAILURE);
	}
	else if (g_pid > 0)
		waitpid(g_pid, &status, WUNTRACED);
	init_signals();
	ft_free_tab_str(arg);
	ft_strdel(&bin);
}
