/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffoissey <ffoissey@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 18:44:18 by ffoissey          #+#    #+#             */
/*   Updated: 2020/05/02 18:47:12 by ffoissey         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "daemon_taskmaster.h"

int     child_process(t_program *prog, pid_t *pgid)
{
	FILE *stderrfile;
	FILE *stdoutfile;

    prog->pid = getpid();
    if (*pgid == 0)
		*pgid = prog->pid;
	else
        prog->pgid = *pgid;
	dprintf(STDERR_FILENO, "Prog %s pid %d pgid %d\n", prog->process_name, prog->pid, prog->pgid);

 	stdoutfile = fopen( prog->stdout_logfile, "a+" );
	stderrfile = fopen( prog->stderr_logfile, "a+" );
    if (stdoutfile == NULL || stderrfile == NULL)
	{
        fputs( "Could not open file", stderr );
        exit (1);
    }
    dup2(fileno(stdoutfile), STDOUT_FILENO);
	dup2(fileno(stderrfile), STDERR_FILENO);
	fclose(stdoutfile);
	fclose(stderrfile);
    if (execve(prog->process_name, prog->args, NULL) == FAILURE)
			ft_dprintf(2, "42sh: execution error\n");
	exit(1);
}

void    start_jobs(t_env *env)
{
    t_list      *ptr;
    t_program   *prog;
	pid_t		pgid;

    ptr = env->prgm_list;
    while (ptr != NULL)
    {
        prog = ptr->content;
		pgid = 0;
        if (prog->autostart == TRUE)
        {
			dprintf(STDERR_FILENO, "Starting %s with %d processes\n", prog->name, prog->numprocs);
			for (int i = 0; i < prog->numprocs; i++)
			{
            	if ((prog->pid = fork()) < 0)
	        	{
	        		dprintf(STDERR_FILENO, "Fork() error\n");
	        		return ;
        		}
	        	else if (prog->pid == 0)
	        		child_process(prog, &pgid);
			}
//        	else
//	        	return (parent_process());*/
        }
        ptr = ptr->next;
    }
}
