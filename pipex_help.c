/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:01:24 by mdziadko          #+#    #+#             */
/*   Updated: 2025/04/14 17:03:46 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	run_fst_cmd(t_pipex *px, int i)
{
	pid_t	pid;

	if (pipe(px->fd.pipe) == -1)
		err("First pipe", px, errno);
	pid = fork();
	if (pid == -1)
		err("First fork", px, errno);
	if (pid == 0)
	{
		if (dup2(px->fd.in, STDIN_FILENO) == -1)
			err("First dup2 while reading", px, errno);
		if (dup2(px->fd.pipe[1], STDOUT_FILENO) == -1)
			err("First dup2 while writting", px, errno);
		close(px->fd.in);
		close(px->fd.pipe[0]);
		close(px->fd.pipe[1]);
		if (execve(px->cmds[i]->path, px->cmds[i]->args, px->envp) == -1)
			err("Execve first cmd", px, errno);
	}
	else
	{
		close(px->fd.pipe[1]);
		px->fd.prev_read = px->fd.pipe[0];
	}
}

void	run_mid_cmd(t_pipex *px, int i)
{
	pid_t	pid;

	if (pipe(px->fd.pipe) == -1)
		err("Mid pipe", px, errno);
	pid = fork();
	if (pid == -1)
		err("Mid fork", px, errno);
	if (pid == 0)
	{
		if (dup2(px->fd.prev_read, STDIN_FILENO) == -1)
			err("Mid dup2 while reading", px, errno);
		if (dup2(px->fd.pipe[1], STDOUT_FILENO) == -1)
			err("Mid dup2 while writting", px, errno);
		close(px->fd.prev_read);
		close(px->fd.pipe[0]);
		close(px->fd.pipe[1]);
		if (execve(px->cmds[i]->path, px->cmds[i]->args, px->envp) == -1)
			err("Execve mid cmd", px, errno);
	}
	else
	{
		close(px->fd.prev_read);
		close(px->fd.pipe[1]);
		px->fd.prev_read = px->fd.pipe[0];
	}
}

void	run_last_cmd(t_pipex *px, int i)
{
	pid_t	pid;

	pid = fork();
	px->last_pid = pid;
	if (pid == -1)
		err("Last fork", px, errno);
	if (pid == 0)
	{
		if (dup2(px->fd.prev_read, STDIN_FILENO) == -1)
			err("Last dup2 while reading", px, errno);
		if (dup2(px->fd.out, STDOUT_FILENO) == -1)
			err("Last dup2 while writting", px, errno);
		close(px->fd.prev_read);
		close(px->fd.out);
		if (execve(px->cmds[i]->path, px->cmds[i]->args, px->envp) == -1)
			err("Execve last cmd", px, errno);
	}
	else
		close(px->fd.prev_read);
}

int	wait_for_children(pid_t last_pid)
{
	int		status;
	pid_t	pid;
	int		exit_code;

	exit_code = 1;
	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
		}
		pid = wait(&status);
	}
	return (exit_code);
}
