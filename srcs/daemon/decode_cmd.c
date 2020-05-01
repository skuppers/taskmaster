/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decode_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 14:51:50 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/01 15:28:35 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

static int		get_tab_size(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i] != NULL)
		i++;
	return ((int)i);
}

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
	cmd->ac = get_tab_size(cmd->av); 
	return (cmd);
}

t_cmd			*decode_cmd(t_vector *trame)
{
	static t_cmd	cmd;
	uint32_t		size;

	ft_bzero(&cmd, sizeof(t_cmd));
	if (vct_getfirstchar(trame) != SOH || vct_getlastchar(trame) != ENQ)
		return (NULL);
	vct_pop(trame);
	size = (*((uint64_t *)vct_getstr(trame)) & 0xffffffff);
	if (size - 1 != vct_len(trame))
	{
		ft_dprintf(STDERR_FILENO, "Error: Trame has bad size\n");
		return (NULL);
	}
	vct_popfrom(trame, 4);
	if ((cmd.type = ((uint8_t)(vct_getfirstchar(trame))) - 128) > NB_CMD)
		return (NULL);
	vct_pop(trame);
	if ((cmd.ocp = (((uint8_t)vct_getfirstchar(trame)) - 128)) < 0)
		return (NULL);
	vct_pop(trame);
	vct_cut(trame);
	return (fill_arg(&cmd, trame));
}
