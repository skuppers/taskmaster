/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 14:51:50 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/04 17:39:30 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "common.h"

static t_cmd	*fill_arg(t_cmd *cmd, t_vector *trame)
{
	if ((vct_getfirstchar(trame) == STX && vct_getlastchar(trame) != ETX)
		|| (vct_getfirstchar(trame) != STX && vct_getlastchar(trame) == ETX))
		return (NULL);
	else if (vct_getfirstchar(trame) != STX)
		return (cmd);
	vct_pop(trame);
	vct_cut(trame);
	cmd->av = ft_strsplit(vct_getstr(trame), US);
	cmd->ac = ft_tabsize(cmd->av); 
	return (cmd);
}

static int		pre_trame(t_vector *trame)
{
	uint32_t		size;

//	debug_print_bytecode(trame);
	if (vct_getfirstchar(trame) != SOH || vct_getlastchar(trame) != ENQ)
	{
		dprintf(STDERR_FILENO, "Bad encapsulation\n");
		return (FAILURE);
	}
	vct_pop(trame);
	size = (*((uint64_t *)vct_getstr(trame)) & 0xffffffff);
	if (size - 1 != vct_len(trame))
	{
		ft_dprintf(STDERR_FILENO, "Error: Trame has bad size\n");
		return (FAILURE);
	}
	vct_popfrom(trame, 4);
	return (SUCCESS);
}

t_vector		*decode_feedback(t_vector *trame)
{
	if (pre_trame(trame) == FAILURE)
		return (NULL);
	vct_cut(trame);
	return (trame);
}

t_cmd			*decode_cmd(t_vector *trame)
{
	static t_cmd	cmd;

	ft_bzero(&cmd, sizeof(t_cmd));
	if (pre_trame(trame) == FAILURE)
		return (NULL);
	if ((cmd.type = ((uint8_t)(vct_getfirstchar(trame))) - 128) > NB_CMD
		|| cmd.type < 0)
		return (NULL);
	vct_pop(trame);
	if ((cmd.ocp = (((uint8_t)vct_getfirstchar(trame)) - 128)) < 0
		|| cmd.ocp > 3)
		return (NULL);
	vct_pop(trame);
	vct_cut(trame);
	return (fill_arg(&cmd, trame));
}
