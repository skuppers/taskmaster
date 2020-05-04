/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/04 22:37:27 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

t_vector	*action_start(t_instance *instance, t_program *program)
{
	(void)sig;
	if (instance != NULL && program != NULL)
	{
		if (instance->state == E_RUNNING || instance->state == E_STARTING)
			return (get_msg(instance->name, "already started", ERR_MSG));
		if (start_instance(program, instance->id, g_env->environ) == SUCCESS)
			return (get_msg(instance->name, "started", INFO_MSG));
	}
	return (get_msg(instance->name, "start-up error", ERR_MSG));
}

t_vector	*action_stop(t_instance *instance, t_program *program)
{
	(void)sig;
	if (instance != NULL && program != NULL)
	{
		if (instance->state != E_RUNNING && instance->state != E_STARTING)
			return (get_msg(instance->name, "already stoped", ERR_MSG));
		if (stop_instance(program, instance, SIGTERM) == SUCCESS)
			return (get_msg(instance->name, "stoped", INFO_MSG));
	}
	return (get_msg(instance->name, "unknow error", ERR_MSG));
}

t_vector	*action_restart(t_instance *instance, t_program *program)
{
	(void)sig;
	if (instance != NULL && program != NULL)
	{
		if (instance->state == E_RUNNING || instance->state == E_STARTING)
			stop_instance(program, instance, SIGTERM);
		if (start_instance(program, instance->id, g_env->environ) == SUCCESS)
			return (get_msg(instance->name, "restarted", INFO_MSG));
	}
	return (get_msg(instance->name, "start-up error", ERR_MSG));
}

t_vector	*action_status(t_instance *instance, t_program *program)
{
	t_vector	*vct;
	char		*str;
	char		*state;

	vct = NULL;
	(void)program;
	(void)sig;
	if (instance != NULL && program != NULL)
	{
		state = get_instance_state(instance);
		str = ft_asprintf("%s\t\t%s\tpid %5d, uptime %d\n", instance->name,
					state, instance->pid, instance->uptime); // A FORMATER EM FONCTION DES STATUTS
		vct = vct_newstr(str);
		ft_strdel(&str);
	}
	return (vct);
}

t_vector	*action_signal(t_instance *instance, t_program *program)
{
	(void)sig;
	if (instance != NULL && program != NULL)
	{
		if (instance->state != E_RUNNING && instance->state != E_STARTING)
			return (get_msg(instance->name,
				"attempting to send signal but it wasn't running", ERR_MSG));
		if (stop_instance(program, instance, g_env->sig_tmp) == SUCCESS)
			return (get_msg(instance->name, "signaled", INFO_MSG));
	}
	return (get_msg(instance->name, "Unknow error", ERR_MSG));
}





t_vector			*cmd_add(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_avail(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_clear(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_fg(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_maintail(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_open(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_pid(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_reload(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_remove(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_reread(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_restart(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_restart);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av, cmd->ac, action_restart);
	return (vct);
}

t_vector			*cmd_shutdown(t_cmd *cmd)
{
	(void)cmd;
	exit_routine();
	return (NULL);
}

t_vector			*cmd_signal(t_cmd *cmd)
{
	t_vector	*vct;

	if (cmd->ac == 1)
		return (NULL);
	if (ft_strcheck(cmd->av[0], ft_isdigit) == FALSE
		|| ft_strlen(cmd->av[0]) > 2 || ft_atoi(cmd->av[0]) > 32)
	{
		vct = vct_new("Bad signal: ");
		vct_addstr(av[0]);
		vct_add('\n');
		return (vct);
	}
	g_env->sig_tmp = ft_atoi(cmd->av[0]); 
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_start);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av + 1, cmd->ac - 1, action_start);
	g_env->sig_tmp = 0; 
	return (NULL);
}

t_vector			*cmd_start(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_start);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av, cmd->ac, action_start);
	return (vct);
}

t_vector			*cmd_status(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_status);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av, cmd->ac, action_status);
	return (vct);
}

t_vector			*cmd_stop(t_cmd *cmd)
{
	t_vector	*vct;

	vct = NULL;
	if (cmd->ocp == 0x01)
		vct = exec_action_all(action_stop);
	else if (cmd->ocp == 0x02)
		vct = exec_action_args(cmd->av, cmd->ac, action_stop);
	return (vct);
}

t_vector			*cmd_tail(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_update(t_cmd *cmd)
{
	(void)cmd;
	return (NULL);
}

t_vector			*cmd_version(t_cmd *cmd)
{
	(void)cmd;
	t_vector *vct = vct_newstr("Version alpha 0.1\n");
	
	return (vct);
}

static int			is_invalid_cmd(enum e_cmd_type type)
{
	return (type >= NB_CMD || type == HELP || type == EXIT || type == QUIT);
}

t_vector			*execute_cmd(t_cmd *cmd)
{
	static t_process_cmd	process_cmd[NB_CMD] = {cmd_add, cmd_avail, cmd_clear,
									NULL, cmd_fg, NULL, cmd_maintail, cmd_open,
									cmd_pid, NULL, cmd_reload, cmd_remove,
									cmd_reread, cmd_restart, cmd_shutdown,
									cmd_signal, cmd_start, cmd_status,
									cmd_stop, cmd_tail, cmd_update,
									cmd_version};

	if (is_invalid_cmd(cmd->type) == TRUE)	
	{
		ft_dprintf(STDERR_FILENO, "Cmd %#.2x is not recognized\n", cmd->type);
		return (NULL);
	}
	return (generate_feedback(process_cmd[cmd->type](cmd)));
}
