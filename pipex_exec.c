/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:31:54 by mdziadko          #+#    #+#             */
/*   Updated: 2025/04/18 10:12:24 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	run_fst_cmd(t_pipex *px, int i)
{
	pid_t	pid;

	if (pipe(px->fd.pipe) == -1)
		err("pipe", px, errno);
	pid = fork();
	if (pid == -1)
		err("fork", px, errno);
	if (pid == 0)
	{
		if (px->fd.in != -1 && dup2(px->fd.in, STDIN_FILENO) == -1)
			err("dup2 in", px, errno);
		if (dup2(px->fd.pipe[1], STDOUT_FILENO) == -1)
			err("dup2 out", px, errno);
		safe_close(&px->fd.in);
		safe_close(&px->fd.pipe[0]);
		safe_close(&px->fd.pipe[1]);
		if (execve(px->cmds[i]->path, px->cmds[i]->args, px->envp) == -1)
			err(*px->cmds[i]->args, px, errno);
	}
	else
	{
		safe_close(&px->fd.pipe[1]);
		safe_close(&px->fd.in);
		px->fd.prev_read = px->fd.pipe[0];
	}
}

void	run_mid_cmd(t_pipex *px, int i)
{
	pid_t	pid;

	if (pipe(px->fd.pipe) == -1)
		err("pipe", px, errno);
	pid = fork();
	if (pid == -1)
		err("fork", px, errno);
	if (pid == 0)
	{
		if (dup2(px->fd.prev_read, STDIN_FILENO) == -1)
			err("inflow redirection", px, errno);
		if (dup2(px->fd.pipe[1], STDOUT_FILENO) == -1)
			err("outlow redirection", px, errno);
		safe_close(&px->fd.prev_read);
		safe_close(&px->fd.pipe[0]);
		safe_close(&px->fd.pipe[1]);
		if (execve(px->cmds[i]->path, px->cmds[i]->args, px->envp) == -1)
			err(*px->cmds[i]->args, px, errno);
	}
	else
	{
		safe_close(&px->fd.prev_read);
		safe_close(&px->fd.pipe[1]);
		px->fd.prev_read = px->fd.pipe[0];
	}
}

void	run_last_cmd(t_pipex *px, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		err("fork", px, errno);
	px->last_pid = pid;
	if (pid == 0)
	{
		if (dup2(px->fd.prev_read, STDIN_FILENO) == -1)
			err("inflow redirection", px, errno);
		if (dup2(px->fd.out, STDOUT_FILENO) == -1)
			err("outlow redirection", px, errno);
		safe_close(&px->fd.prev_read);
		safe_close(&px->fd.out);
		if (execve(px->cmds[i]->path, px->cmds[i]->args, px->envp) == -1)
			err(*px->cmds[i]->args, px, errno);
	}
	else
		safe_close(&px->fd.prev_read);
}

void	run_pipe(t_pipex *px)
{
	int		i;

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
}

int	execute_pipex(t_pipex *px)
{
	px->fd.in = open(px->infile, O_RDONLY);
	if (px->fd.in == -1)
		perror(px->infile);
	px->fd.out = open(px->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (px->fd.out == -1)
		err(px->outfile, px, 1);
	run_pipe(px);
	if (px->fd.in != -1)
		close(px->fd.in);
	if (px->fd.out != -1)
		close(px->fd.out);
	return (wait_for_children(px->last_pid));
}
