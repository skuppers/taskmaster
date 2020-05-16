/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/14 18:51:44 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

uint8_t			get_startretries(uint8_t *err, dictionary *d, char *name)
{
	int32_t		get;

	get = get_secint(d, name, ":startretries", 0);
	if (get == 0)
		get = 3;
	if (!is_in_range(get, 1, 255))
	{
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - startretries is not in"
			" range: 1-255\n", name);
		*err = 1;
		return (0);
	}
	return ((uint8_t)get);
}

uint8_t			get_stopwaitsecs(uint8_t *err, dictionary *d, char *name)
{
	int32_t		get;

	get = get_secint(d, name, ":stopwaitsecs", 0);
	if (get == 0)
		get = 3;
	if (!is_in_range(get, 1, 255))
	{
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - stopwaitsecs is not in "
			" range: 1-255\n", name);
		*err = 1;
		return (0);
	}
	return ((uint8_t)get);
}

uint8_t			get_stopsig(uint8_t *err, dictionary *d, char *name)
{
	char		*get;
	int			i;
	static char	*sigs[] = {"TERM", "HUP", "INT", "QUIT", "KILL", "USR1",
							"USR2"};
	static int	signo[] = {SIGTERM, SIGHUP, SIGINT, SIGQUIT, SIGKILL,
							SIGUSR1, SIGUSR2};

	get = get_secstring(d, name, ":stopsignal");
	if (get == NULL)
		get = sigs[0];
	i = 0;
	while (i < 7)
	{
		if (ft_strequ(get, sigs[i]) == TRUE)
			return (signo[i]);
		++i;
	}
	dprintf(STDERR_FILENO, "taskmasterd: [%s] - stopsignal must be one of : "
			"TERM, HUP, INT, QUIT, KILL, USR1, or USR2.\n", name);
	*err = 1;
	return (0);
}

int16_t			get_userid(uint8_t *err, dictionary *d, char *name)
{
	char		*get;

	get = get_secstring(d, name, ":userid");
	if (get == NULL)
		return (FAILURE);
	if (ft_strlen(get) == 0)
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - userid field cannot"
			" be blank.\n", name);
	else if (ft_strcheck(get, ft_isdigit) != TRUE)
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - userid field must be"
			" numeric.\n", name);
	else if (ft_strlen(get) >= 6 || ft_atoi(get) < 0 || ft_atoi(get) > 16535)
		dprintf(STDERR_FILENO, "taskmasterd: [%s] -  userid is not in "
			"range 0-16535\n", name);
	else
		return (ft_atoi(get));
	*err = 1;
	return (FAILURE);
}

char			*get_directory(uint8_t *err, dictionary *d, char *name)
{
	char		*get;

	get = get_secstring(d, name, ":directory");
	if (get == NULL)
		return (NULL);
	if (ft_strlen(get) == 0)
	{
		*err = 1;
		dprintf(STDERR_FILENO, "taskmasterd: [%s] - directory field "
			"cannot be blank.\n", name);
		return (NULL);
	}
	return (get);
}
