/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   deamon.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuppers <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 15:13:23 by skuppers          #+#    #+#             */
/*   Updated: 2020/02/15 17:47:05 by skuppers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef FT_DEAMON_H
# define FT_DEAMON_H

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>

# define SOCKET_PATH "taskmtr"


void    					listen_for_controller(void);

#endif
