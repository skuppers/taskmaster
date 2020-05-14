/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/13 17:15:07 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

uint16_t		get_priority(uint8_t *err, dictionary *d, char *name)
{
	int32_t		get;

	get = get_secint(d, name, ":priority", 0);
	if (get == 0)
		get = 999;
	if (!is_in_range(get, 1, 999))
	{
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - priority is not in"
			" range: 1-999\n", name);
		*err = 1;
		return (0);
	}
	return ((uint16_t)get);
}

char			*get_stdoutlog(uint8_t *err, dictionary *d, char *name)
{
	char		*get;

	get = get_secstring(d, name, ":stdout_logfile");
	if (get == NULL)
		return ("AUTO");
	if (ft_strlen(get) == 0)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - stdout_logfile field "
			"cannot be blank.\n", name);
		return (NULL);
	}
	return (get);
}

char			*get_stderrlog(uint8_t *err, dictionary *d, char *name)
{
	char		*get;

	get = get_secstring(d, name, ":stderr_logfile");
	if (get == NULL)
		return ("AUTO");
	if (ft_strlen(get) == 0)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - stderr_logfile field "
			"cannot be blank.\n", name);
		return (NULL);
	}
	return (get);
}

char			*get_environement(uint8_t *err, dictionary *d, char *name)
{
	char		*get;

	get = get_secstring(d, name, ":environment");
	if (get == NULL)
		return (NULL);
	if (ft_strlen(get) == 0)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - environment field cannot "
			"be blank.\n", name);
		return (NULL);
	}
	return (get);
}

mode_t			get_umask(uint8_t *err, dictionary *d, char *name)
{
	char		*get;

	get = get_secstring(d, name, ":umask");
	if (get == NULL)
		return ((mode_t)strtol("0", NULL, 8));
	if (ft_strlen(get) == 0)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - umask field cannot "
			"be blank.\n", name);
		return (0);
	}
	if (ft_strcheck(get, ft_isdigit) != TRUE)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [taskmasterd] - umask field "
			"must be numeric.\n");
	}
	return ((mode_t)strtol(get, NULL, 8));
}
