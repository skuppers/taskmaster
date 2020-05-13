/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_avail.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 02:28:44 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/05 02:28:52 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"


static void			do_stuff_on_instance(t_vector *msg, t_program *p)
{
	t_instance	*in;
	char		*tmp;

	in = p->instance;
	while (in != NULL)
	{
		tmp = ft_asprintf("%s:%d\t\t\tin use\t%s\t%d\n",
							p->name, in->id, (p->autostart == 1) ? "auto": "prout",
							p->userid);
		vct_addstr(msg, tmp);
		in = in->next;
	}
}
static void			do_stuff_on_instance2(t_vector *msg, t_program *p)
{
	t_instance	*in;
	char		*tmp;

	in = p->instance;
	while (in != NULL)
	{
		tmp = ft_asprintf("%s:%d\t\t\tavail\t%s\t%d\n",
							p->name, in->id, (p->autostart == 1) ? "auto": "prout",
							p->userid);
		vct_addstr(msg, tmp);
		in = in->next;
	}
}

t_vector			*action_avail(void)
{
	t_vector	*msg;
	t_list		*lptr;
	t_program	*pptr;

	msg = vct_newstr("");
	lptr = g_denv->prgm_list;
	while (lptr != NULL)
	{
		pptr = lptr->content;
		if (pptr->availmode == E_LOCKED || pptr->availmode == E_CHANGED)
		{
			do_stuff_on_instance(msg, pptr);
		}
		lptr = lptr->next;
	}
	if (g_tmpenv == NULL)
		return (msg);
	lptr = g_tmpenv->prgm_list;
	while (lptr != NULL)
	{
		pptr = lptr->content;
		if (pptr->availmode == E_ADDED)
		{
			do_stuff_on_instance2(msg, pptr);
		}
		lptr = lptr->next;
	}
	return (msg);
}

t_vector			*cmd_avail(t_cmd *cmd)
{
	(void)cmd;
	return (action_avail());
}
