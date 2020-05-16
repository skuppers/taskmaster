/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/15 22:06:37 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

void			get_exitcodes(uint8_t *err, t_program *prog, dictionary *dict,
					char *secname)
{
	int			i;
	char		*to_split;

	to_split = get_secstring(dict, secname, ":exitcodes");
	if (to_split == NULL)
		to_split = "0";
	prog->exitcodes = ft_strsplit(to_split, ',');
	i = 0;
	while (*err != 1 && prog->exitcodes[i] != NULL)
	{
		if (ft_strlen(prog->exitcodes[i]) > 3)
		{
			*err = 1;
			dprintf(STDERR_FILENO, "taskmasterd: [%s] - exitcodes is not"
				" in range 0-255\n", secname);
		}
		else if (is_in_range(ft_atoi(prog->exitcodes[i]), 0, 255) == FALSE)
		{
			*err = 1;
			dprintf(STDERR_FILENO, "taskmasterd: [%s] - exitcodes is not"
				" in range 0-255\n", secname);
		}
		++i;
	}
}

uint8_t			get_autorestart(uint8_t *error, dictionary *dict, char *secname)
{
	char		*str;

	str = get_secstring(dict, secname, ":autorestart");
	if (str == NULL)
		return (UNEXPECTED);
	if (ft_strequ(str, "true") || ft_strequ(str, "TRUE") || ft_strequ(str, "1"))
		return (TRUE);
	if (ft_strequ(str, "false") || ft_strequ(str, "FALSE")
		|| ft_strequ(str, "0"))
		return (FALSE);
	if (ft_strequ(str, "unexpected") || ft_strequ(str, "UNEXPECTED"))
		return (UNEXPECTED);
	dprintf(STDERR_FILENO, "taskmasterd: [%s] - autorestart does not match "
			"known instructions: TRUE / FALSE / UNEXPECTED\n", secname);
	*error = 1;
	return (FALSE);
}

uint8_t			get_numprocs(uint8_t *error, dictionary *d, char *name)
{
	int32_t		get;

	get = get_secint(d, name, ":numprocs", 0);
	if (get == 0)
		return (1);
	if (!is_in_range(get, 1, 255))
	{
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - numprocs is not in range"
			" 1-255: %d\n", name, get);
		*error = 1;
		return (0);
	}
	return ((uint8_t)get);
}

char			*get_command(uint8_t *error, dictionary *d, char *name)
{
	char		*cmd;

	cmd = get_secstring(d, name, ":command");
	if (cmd == NULL || ft_strlen(cmd) == 0)
	{
		dprintf(STDERR_FILENO, "[%s] - You need to specify a command!\n", name);
		*error = 1;
		return (NULL);
	}
	return (cmd);
}

uint8_t			get_startsecs(uint8_t *error, dictionary *d, char *name)
{
	int32_t		get;

	get = get_secint(d, name, ":startsecs", 0);
	if (get == 0)
		get = 0;
	if (!is_in_range(get, 0, 255))
	{
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - startsecs is not in range:"
			" 0	- 255\n", name);
		*error = 1;
		return (0);
	}
	return ((uint8_t)get);
}
