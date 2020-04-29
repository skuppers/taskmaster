/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listener.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skuppers <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/03 15:13:23 by skuppers          #+#    #+#             */
/*   Updated: 2020/02/15 17:47:05 by skuppers         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "deamon.h"

static int  				prepare_unsocket(struct sockaddr_un *addr)
{
	int fd;

	if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket error");
		exit(-1);
	}
	memset(addr, 0, sizeof(addr));
	addr->sun_family = AF_UNIX;
   	strncpy(addr->sun_path, SOCKET_PATH, sizeof(addr->sun_path));
	unlink(SOCKET_PATH);
	return (fd);
}

static int bind_socket(int fd, struct sockaddr_un *addr)
{
	if (bind(fd, (struct sockaddr*)addr, sizeof(addr)) == -1)
	{
    	perror("bind error");
    	exit(-1);
  	}
	if (listen(fd, 2) == -1)
	{
    	perror("listen error");
    	exit(-1);
  	}
	return (0);
}

static void		receive_data(int fd)
{
	int clientfd;
	int	rc;
	char buf[100];

	while (1)
	{
   		if ( (clientfd = accept(fd, NULL, NULL)) == -1) {
     		perror("accept error");
      		continue;
    	}

    	while ((rc = read(clientfd, buf, sizeof(buf))) > 0)
    		printf("read %u bytes: %.*s\n", rc, rc, buf);

    	if (rc == -1)
		{
     	 perror("read");
     	 exit(-1);
  	  }
    	else if (rc == 0)
		{
     	 printf("EOF\n");
    	  close(clientfd);
    	}
  	}
}

void    					listen_for_controller(void)
{
	int						fd;
    struct sockaddr_un		addr;

	fd = prepare_unsocket(&addr);
	if (bind_socket(fd, &addr) != 0)
		return ;
	daemon(0, 0);
	receive_data(fd);
}