/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_signal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:14:42 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 19:13:05 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

const char		*get_sigstr(int sig)
{
	static const char	*sigstr[32] = {"", "SIGHUP", "SIGINT", "SIGQUIT",
							"SIGILL", "SIGTRAP", "SIGABRT", "SIGBUS", "SIGFPE",
							"SIGKILL", "SIGUSR1", "SIGSEGV", "SIGUSR2",
							"SIGPIPE", "SIGALRM", "SIGTERM", "SIGSTKFLT",
							"SIGCHLD", "SIGCONT", "SIGSTOP", "SIGTSTP",
							"SIGTTIN", "SIGTTOU", "SIGURG", "SIGXCPU",
							"SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH",
							"SIGPOLL", "SIGPWR", "SIGSYS"};
	
	return (sig > 0 && sig < 32 ? sigstr[sig] : NULL);
}

t_vector	*action_signal(t_instance *instance, t_program *program)
{
	t_vector	*vct;
	char		*str;

	vct = NULL;
	if (instance != NULL && program != NULL
		&& g_env->sig_tmp > 0 && g_env->sig_tmp < 32)
	{
		str = NULL;
		if (instance->state != E_RUNNING && instance->state != E_STARTING)
		{
			str = ft_asprintf("attempting to send signal"
					" `%s' but it wasn't running", get_sigstr(g_env->sig_tmp));
			vct = get_msg(instance->name, str, ERR_MSG);
		}
		else if (stop_instance(program, instance, g_env->sig_tmp) == SUCCESS)
		{
			str = ft_asprintf("signaled by %s", get_sigstr(g_env->sig_tmp));
			vct = get_msg(instance->name, str, INFO_MSG);
		}
		ft_strdel(&str);
	}
	if (vct == NULL)
		vct = get_msg(instance == NULL ? "???" : instance->name,
			"bad signal", ERR_MSG);
	return (vct);
}

int					get_signal_number(char *arg)
{
	int			sig;

	sig = 0;
	if (ft_strcheck(arg, ft_isdigit) == FALSE)
	{
		sig = 1;
		ft_strupcase(arg);
		while (sig < 32)
		{
			if (ft_strequ(arg, get_sigstr(sig)) == TRUE)
				return (sig);
			sig++;
		}
	}
	else if (ft_strlen(arg) <= 2)
		sig = ft_atoi(arg);
	return (sig > 0 && sig < 32 ? sig : 0);
}

t_vector			*cmd_signal(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ac == 0)
		return (NULL);
	g_env->sig_tmp = get_signal_number(cmd->av[0]);
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_signal);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av + 1, cmd->ac - 1, action_signal);
	g_env->sig_tmp = 0; 
	return (vct);
}
