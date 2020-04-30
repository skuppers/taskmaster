/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:14:48 by ffoissey          #+#    #+#             */
/*   Updated: 2020/04/29 19:05:55 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "daemon_taskmaster.h"

void		print_log(t_env *env, const char *priority, const char *message, ...)
{
	va_list args;

	ft_dprintf(env->log_fd, "%s- ", priority);
	va_start(args, message);
	ft_vdprintf(message, args, env->log_fd);
	va_end(args);
}

int8_t      init_log(t_env *env)
{
    int32_t     debug_fd;

    debug_fd = open(DFLT_LOGPATH, O_RDWR | O_APPEND | O_CREAT, 0644);
	if (debug_fd < 0)
	{
		printf("Could not create log file.\n");
		return (-1);
	}
	env->log_fd = debug_fd;
	return (0);
}