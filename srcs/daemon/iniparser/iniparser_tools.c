/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iniparser_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 17:34:24 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/03 17:34:31 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

char	*get_secstring(dictionary *dict, char *secname, char *key)
{
	t_vector	*buffer;
	char		*retval;

	buffer = vct_newstr(secname);
	vct_addstr(buffer, key);
	retval = (char *)iniparser_getstring(dict, vct_getstr(buffer), NULL);
	vct_del(&buffer);
	return (retval);
}

int8_t	get_secbool(dictionary *dict, char *secname, char *key)
{
	t_vector	*buffer;
	int8_t		retval;

	buffer = vct_newstr(secname);
	vct_addstr(buffer, key);
	retval = (int8_t)iniparser_getboolean(dict, vct_getstr(buffer), 0);
	vct_del(&buffer);
	return (retval);
}

int32_t	get_secint(dictionary *dict, char *secname, char *key)
{
	t_vector	*buffer;
	int32_t		retval;

	buffer = vct_newstr(secname);
	vct_addstr(buffer, key);
	retval = (int32_t)iniparser_getint(dict, vct_getstr(buffer), 0);
	vct_del(&buffer);
	return (retval);
}

dictionary *load_ini_file(char *str)
{
    dictionary	*ini_dict;

	ini_dict = iniparser_load(str);
	if (ini_dict == NULL)
	{
		printf("Could not read ini file: %s\n", strerror(errno));
        exit_routine();
	}
//	iniparser_dump(ini_dict, stdout);
	return (ini_dict);
}
