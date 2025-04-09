/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:31:54 by mdziadko          #+#    #+#             */
/*   Updated: 2025/04/09 13:25:45 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	run_fst_cmd(t_pipex *px, int i)
{
	int	pid;

	if (pipe(px->fd.pipe) == -1)
		err("First pipe", px);
	pid = fork();
	if (pid == -1)
		err("First fork", px);
	if (pid == 0)
	{
		if (dup2(px->fd.in, STDIN_FILENO) == -1)
			err("First dup2 while reading", px);
		if (dup2(px->fd.pipe[1], STDOUT_FILENO) == -1)
			err("First dup2 while writting", px);
		close(px->fd.in);
		close(px->fd.pipe[0]);
		close(px->fd.pipe[1]);
		if (execve(px->cmds[i]->path, px->cmds[i]->args, px->envp) == -1)
			err("Execve first cmd", px);
	}
	else
	{
		close(px->fd.pipe[1]);
		px->fd.prev_read = px->fd.pipe[0];
	}
}

void	run_mid_cmd(t_pipex *px, int i)
{
	int	pid;

	if (pipe(px->fd.pipe) == -1)
		err("Mid pipe", px);
	pid = fork();
	if (pid == -1)
		err("Mid fork", px);
	if (pid == 0)
	{
		if (dup2(px->fd.prev_read, STDIN_FILENO) == -1)
			err("Mid dup2 while reading", px);
		if (dup2(px->fd.pipe[1], STDOUT_FILENO) == -1)
			err("Mid dup2 while writting", px);
		close(px->fd.prev_read);
		close(px->fd.pipe[0]);
		close(px->fd.pipe[1]);
		if (execve(px->cmds[i]->path, px->cmds[i]->args, px->envp) == -1)
			err("Execve mid cmd", px);
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
	int	pid;

	pid = fork();
	if (pid == -1)
		err("Last fork", px);
	if (pid == 0)
	{
		if (dup2(px->fd.prev_read, STDIN_FILENO) == -1)
			err("Last dup2 while reading", px);
		if (dup2(px->fd.out, STDOUT_FILENO) == -1)
			err("Last dup2 while writting", px);
		close(px->fd.prev_read);
		close(px->fd.out);
		if (execve(px->cmds[i]->path, px->cmds[i]->args, px->envp) == -1)
			err("Execve last cmd", px);
	}
	else
		close(px->fd.prev_read);
}

void	run_pipe(t_pipex *px)
{
	int	i;
	int	j;

	i = 0;
	while (i < px->cmds_count)
	{
		if (i == 0)
			run_fst_cmd(px, i);
		else if (i < px->cmds_count - 1)
			run_mid_cmd(px, i);
		else
			run_last_cmd(px, i);
		i++;
	}
	j = 0;
	while (j++ < px->cmds_count)
		wait(NULL);
}

int	execute_pipex(t_pipex *px)
{
	px->fd.in = open(px->infile, O_RDONLY);
	if (px->fd.in == -1)
		err("Open infile", px);
	px->fd.out = open(px->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (px->fd.out == -1)
		err("Open outfile", px);
	px->fd.prev_read = -1;
	run_pipe(px);
	close(px->fd.in);
	close(px->fd.out);
	return (0);
}
