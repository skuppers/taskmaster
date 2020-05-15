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

static void     sigint_handle(int signo)
{
    dprintf(2, "Received int signal %d but ignoring\n", signo);
}

int main(void)
{
    struct sigaction	sigint;

	sigemptyset(&sigint.sa_mask);
	sigint.sa_handler = sigint_handle;
	sigint.sa_flags = 0;
	sigaction(SIGINT, &sigint, NULL);

    while (1)
        sleep(1);

    return (0);
}
