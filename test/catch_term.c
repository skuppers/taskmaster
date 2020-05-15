/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuppers <skuppers@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/15 17:11:01 by skuppers          #+#    #+#             */
/*   Updated: 2020/05/15 17:11:06 by skuppers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void     sigterm_handle(int signo)
{
    dprintf(2, "Received term signal %d but ignoring\n", signo);
}

int main(void)
{
    struct sigaction	sigterm;

	sigemptyset(&sigterm.sa_mask);
	sigterm.sa_handler = sigterm_handle;
	sigterm.sa_flags = 0;
	sigaction(SIGTERM, &sigterm, NULL);

    while (1)
        sleep(1);

    return (0);
}
