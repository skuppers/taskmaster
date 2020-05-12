/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_routine.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/10 11:41:20 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 22:24:35 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

pid_t		pid;

static t_vector	*get_trame(t_cmd *cmd)
{
	static	t_builtin	builtin[] = {blt_add, blt_avail, blt_clear, blt_exit,
									blt_fg, blt_help, blt_maintail, blt_open,
									blt_pid, blt_quit, blt_reload, blt_remove,
									blt_reread, blt_restart, blt_shutdown,
									blt_signal, blt_start, blt_status, blt_stop,
									blt_tail, blt_update, blt_version};
	t_vector			*trame;

	trame = builtin[cmd->type](cmd);
	if (g_env->opt.mask & OPT_DEBUG && trame != NULL)
		debug_print_bytecode(trame);
	return (trame);
}


void	sig_int_hand(int signo)
{
	(void)signo;
	kill(pid, SIGINT);
	dprintf(STDERR_FILENO, "^C\n");
}

void	exec_tail(uint64_t flag, t_vector *feedback)
{
	char		**arg;
	int			status;
	char		*bin;
	t_vector	*vct;

	pid = 0;
	(void)flag;
	arg = (char **)malloc(sizeof(char *) * 4);
	bin = ft_strdup("tail");
	arg[0] = ft_strdup("tail");
	if (flag & TAIL_FIFO)
		arg[1] = ft_strdup("-f");
	else
		arg[1] = ft_itoa((int)(-((uint32_t)(flag >> 8))));
	arg[2] = vct_dupstr(feedback);
	arg[3] = NULL;
	if (flag & TAIL_FIFO)
		ft_dprintf(STDERR_FILENO, "==> Press CTRL+C to exit <==\n");
	signal(SIGINT, &sig_int_hand);
	if ((pid = fork()) == 0)
	{
		vct = vct_newstr(getenv("PATH"));
		if (get_new_bin_path(&bin, vct) == SUCCESS)
			execve(bin, arg, NULL);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
		waitpid(pid, &status, 0);
	init_signals();
	ft_free_tab_str(arg);
	ft_strdel(&bin);
}

static t_vector		*send_and_receive(t_vector *trame, const uint8_t flag)
{
	t_vector		*feedback;

	feedback = NULL;
	if (try_to_send_trame(g_env->unix_socket, trame,
							TO_PRINT, ft_dprintf) == SUCCESS)
	{
		feedback = get_feedback();
		if (feedback == NULL && flag == DEL_FEEDBACK)
		{
			dprintf(STDERR_FILENO, "Got no feedback from daemon!\n");
			return (NULL);
		}
		if (g_env->flag_exec != 0 && vct_getlastchar(feedback) == 'g')
			exec_tail(g_env->flag_exec, feedback);	
		else if (flag == DEL_FEEDBACK)
			vct_print_fd(feedback, STDERR_FILENO);
	}
//	else error ? 
	return (feedback);
}

void		restart_connection()
{
	int		i;

	i = RESTART_TIME;
	close(g_env->unix_socket);
	canonic_mode(true);
	dprintf(2, "Reconnection in %d ", RESTART_TIME);
	while (i > 0)
	{
		sleep(1);
		i--;
		dprintf(2, "%d%c", i, (i == 0) ? '\n' : ' ');
	}
	connect_to_daemon(g_env->opt.str[SERVERURL]);
	canonic_mode(false);
}

t_vector	*routine(t_vector *line, const uint8_t flag)
{
	t_vector		*trame;
	t_vector		*feedback;

	g_env->flag_exec = 0;
	g_env->cmd = get_cmd(line);
	trame = NULL;
	feedback = NULL;
	if (g_env->cmd != NULL)
	{
		trame = get_trame(g_env->cmd);
		if (trame != NULL)
			feedback = send_and_receive(trame, flag);
		if (feedback != NULL && g_env->cmd->type == RELOAD)
			restart_connection();
	}
	vct_del(&trame);
	reset_cmd();
	if (flag == DEL_FEEDBACK)
		vct_del(&feedback);
	return (feedback);
}

void			read_cmd(void)
{
	t_vector	*line;
	t_vector	*split;
	int			ret;

	line = vct_new(DFL_VCT_SIZE);
	get_status(DEL_FEEDBACK);
	while ((ret = tsk_readline(line, STDIN_FILENO, g_env)) >= 0)
	{
		if (vct_apply(line, IS_SPACE) == false)
		{
			history(line, ADD_ENTRY | RESET);
			while ((split = vct_split(line, ";", NO_SEP)) != NULL)
			{
				routine(split, DEL_FEEDBACK);
				vct_del(&split);
			}
			vct_split(NULL, NULL, INIT);
		}
	}
	vct_del(&line);
	if (ret == FAILURE)
		exit_routine(ERR, strerror(errno));
}
