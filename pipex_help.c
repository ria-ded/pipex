/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:01:24 by mdziadko          #+#    #+#             */
/*   Updated: 2025/04/17 16:33:02 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

t_pipex	*pipex_init(int argc, char **argv, char **envp)
{
	t_pipex	*px;

	px = malloc(sizeof(t_pipex));
	if (!px)
		return (NULL);
	px->fd.in = -1;
	px->fd.out = -1;
	px->fd.prev_read = -1;
	px->fd.pipe[0] = -1;
	px->fd.pipe[1] = -1;
	if (parse_args(px, argc, argv, envp))
	{
		free (px);
		return (NULL);
	}
	return (px);
}

int	wait_for_children(pid_t last_pid)
{
	int		status;
	pid_t	pid;
	int		exit_code;

	exit_code = 1;
	while (1)
	{
		pid = wait(&status);
		if (pid <= 0)
			break ;
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
		}
	}
	return (exit_code);
}

void	safe_close(int *fd)
{
	if (*fd != -1)
	{
		close(*fd);
		*fd = -1;
	}
}
