/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 15:02:12 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/09 19:45:20 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

static uint8_t	is_rel_or_abs_path(char *cmd)
{
	return ((cmd[0] == '/'
			|| ft_strnequ("./", cmd, 2) || ft_strnequ("../", cmd, 3)));
}

static int8_t	bin_validation(char *bin)
{
	struct stat		stat_info;

	if (access(bin, F_OK) == FAILURE)
		return (FAILURE);
	if (stat(bin, &stat_info) == SUCCESS)
	{
		if (S_ISREG(stat_info.st_mode) && access(bin, X_OK) == SUCCESS)
			return (SUCCESS);
	}
	return (FAILURE);
}

static void		rename_prog(t_program *prog, char *new_name)
{
	ft_strdel(&prog->avs[0]);
	prog->avs[0] = ft_strdup(new_name); // Laisser argv[0] et changer seulement bin ?
	prog->bin = prog->avs[0];
}

int8_t			get_new_bin_path(t_program *prog, t_list *env)
{
	t_vector	*path_vct;
	t_vector	*path_split;
	int8_t		ret;

	ret = SUCCESS;
	path_vct = NULL;
	if (prog != NULL && is_rel_or_abs_path(prog->bin) == FALSE
		 && bin_validation(prog->bin) == FAILURE
		&& (path_vct = vct_newstr(get_var(env, "PATH"))) != NULL)
	{
		ret = FAILURE;
		while (ret == FAILURE
			&& (path_split = vct_split(path_vct, ":", NO_SEP)) != NULL)
		{
			if (vct_getlastchar(path_split) != '/')
				vct_add(path_split, '/');
			vct_addstr(path_split, prog->bin);
			if ((ret = bin_validation(vct_getstr(path_split))) == SUCCESS)
				rename_prog(prog, vct_getstr(path_split));
			vct_del(&path_split);
		}
	}
	vct_del(&path_vct);
	return (ret);
}
