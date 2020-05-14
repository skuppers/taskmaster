/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_avail.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:28:44 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 17:46:38 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static void			print_instance_avail(t_vector *msg, t_program *p,
						char *state)
{
	t_instance	*in;
	char		*tmp;

	in = p->instance;
	while (in != NULL)
	{
		tmp = ft_asprintf("%-20s%s\t%s\t%d\n",
							in->name, state,
							(p->autostart == 1) ? "auto" : "manual",
							(p->userid == -1) ? (int16_t)getuid() : p->userid);
		vct_addstr(msg, tmp);
		in = in->next;
	}
}

t_vector			*action_avail(t_list *lptr, const uint8_t flag)
{
	t_vector	*msg;
	t_program	*pptr;

	msg = vct_newstr("");
	while (lptr != NULL)
	{
		pptr = lptr->content;
		if (flag == TMPENV && pptr->availmode == E_ADDED)
			print_instance_avail(msg, pptr, "avail");
		else if (flag == DENV &&
			(pptr->availmode == E_LOCKED || pptr->availmode == E_CHANGED))
			print_instance_avail(msg, pptr, "in use");
		lptr = lptr->next;
	}
	return (msg);
}

t_vector			*cmd_avail(t_cmd *cmd)
{
	t_vector	*msg;
	t_vector	*output;

	(void)cmd;
	output = action_avail(g_denv->prgm_list, DENV);
	if (g_tmpenv != NULL)
	{
		msg = action_avail(g_tmpenv->prgm_list, TMPENV);
		vct_cat(output, msg);
		vct_del(&msg);
	}
	return (output);
}
