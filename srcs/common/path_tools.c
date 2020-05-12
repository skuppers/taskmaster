/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 15:02:12 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/12 21:07:47 by ffoissey         ###   ########.fr       */
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

static void		rename_prog(char **bin, char *new_name)
{
	ft_strdel(bin);
	*bin = ft_strdup(new_name);
}

int8_t			get_new_bin_path(char **bin, t_vector *path)
{
	t_vector	*path_split;
	int8_t		ret;

	ret = SUCCESS;
	if (is_rel_or_abs_path(*bin) == FALSE
		 && bin_validation(*bin) == FAILURE && path != NULL)
	{
		ret = FAILURE;
		while (ret == FAILURE
			&& (path_split = vct_split(path, ":", NO_SEP)) != NULL)
		{
			if (vct_getlastchar(path_split) != '/')
				vct_add(path_split, '/');
			vct_addstr(path_split, *bin);
			if ((ret = bin_validation(vct_getstr(path_split))) == SUCCESS)
				rename_prog(bin, vct_getstr(path_split));
			vct_del(&path_split);
		}
	}
	vct_del(&path);
	return (ret);
}
