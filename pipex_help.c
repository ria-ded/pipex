/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:01:24 by mdziadko          #+#    #+#             */
/*   Updated: 2025/04/25 17:53:24 by mdziadko         ###   ########.fr       */
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
	px->pids = malloc(sizeof(pid_t) * px->cmds_count);
	if (!px->pids)
	{
		free_pipex(px);
		return (NULL);
	}
	return (px);
}

int	wait_for_children(t_pipex *px)
{
	int	status;
	int	i;
	int	exit_code;

	exit_code = 1;
	i = 0;
	while (i < px->cmds_count)
	{
		if (waitpid(px->pids[i], &status, 0) == -1)
			perror("Waitpid");
		else if (i == px->cmds_count - 1)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
		}
		i++;
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

void	safe_dup2(int old_fd, int new_fd, t_pipex *px)
{
	if (dup2(old_fd, new_fd) == -1)
		err("Dup2 failed", px, errno);
}
