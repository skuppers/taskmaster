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

static void     sigquit_handle(int signo)
{
    dprintf(2, "Received quit signal %d but ignoring\n", signo);
}

int main(void)
{
    struct sigaction	sigquit;

	sigemptyset(&sigquit.sa_mask);
	sigquit.sa_handler = sigquit_handle;
	sigquit.sa_flags = 0;
	sigaction(SIGQUIT, &sigquit, NULL);

    while (1)
        sleep(1);

    return (0);
}
