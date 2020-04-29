/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_taskmaster.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 12:25:42 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client_taskmaster.h"

static char *get_keyword(uint8_t i)
{
	static char		*grammar[] = {"add", "avail", "clear", "exit", "fg",
								"maintail", "open", "pid", "quit", "reload",
								"remove", "reread", "restart", "shutdown",
								"signal", "start", "status", "stop", "tail",
								"update", "version"};

	return (grammar[i]);
}

static enum e_cmd_type	get_cmd(t_vector *word)
{
	uint8_t	i;

	i = 0;
	while (i < NB_CMD)
	{
		if (ft_strequ(vct_getstr(word), get_keyword(i)) == TRUE)
			return ((enum e_cmd_type)i);
		i++;
	}
	ft_dprintf(STDERR_FILENO, "*** Unknow syntax: %s", vct_getstr(word)); // print usage
	return (BAD_CMD);
}

static void	debug_cmd(t_cmd *cmd)
{
	t_list	*lst;
	int		i;
	char	*arg;

	ft_printf("cmd->type = %d (%s)%s",
			(int)cmd->type, get_keyword((uint8_t)cmd->type),
				cmd->ac == 1 ? "\n" : " || "); // DEBUG
	lst = cmd->arg;
	i = 1;
	while (lst != NULL)
	{
		arg = (char *)lst->content;
		ft_printf("arg[%d] = %s%s", i, arg, i == cmd->ac ? "\n" : " | "); // DEBUG
		lst = lst->next;
		i++;
	}
}

static int	parser(t_vector *line)
{
	t_vector	*split;
	t_list		*node;
	t_cmd		cmd;
	size_t		i;

	i = 0;
	vct_split(NULL, NULL, INIT);
	ft_bzero(&cmd, sizeof(t_cmd));
	while ((split = vct_split(line, " \t", NO_SEP)) != NULL)
	{
		if (i == 0)
		{
			if ((cmd.type = get_cmd(split)) == BAD_CMD)
				return (FAILURE);
		}
		else
		{
			node = ft_lstnew(vct_getstr(split), vct_len(split) + 1); // split error
			ft_lstadd_back(&cmd.arg, node);
			// envoyer a chaque builtin
			
		}
		i++;
		vct_del(&split);
	}
	cmd.ac = i;
	debug_cmd(&cmd);
	return (SUCCESS);
}

static int	print_prompt(void)
{
	return (ft_dprintf(STDERR_FILENO, PROMPT));
}

static void read_cmd(t_env *env)
{
	t_vector	*line;
	int			ret;

	line = vct_new(DFL_VCT_SIZE);
	print_prompt();
	while ((ret = tsk_readline(line, STDIN_FILENO, env)) > 0)
	{
		if (vct_apply(line, IS_SPACE) == FALSE)
		{
			parser(line);
			ft_putchar('\n'); // DEBUG
		}
		print_prompt();
	}
	vct_del(&line);
	//if (ret == FAILURE) read_error
}

int		main(void)
{
	t_env	environment;

	if (isatty(STDIN_FILENO) == FALSE)
	{
		ft_dprintf(STDERR_FILENO, "Not a tty\n");
		return (EXIT_FAILURE);
	}

	create_termmode(&environment);
	set_termmode(&environment);
	assign_keycodes(&environment);

	read_cmd(&environment);

	release_termmode(&environment);
	return (EXIT_SUCCESS);
}
