/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:31:51 by mdziadko          #+#    #+#             */
/*   Updated: 2025/04/18 11:36:57 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_cmds(t_cmd **cmds)
{
	int	i;

	if (!cmds)
		return ;
	i = 0;
	while (cmds[i])
	{
		free_cmd(cmds[i]);
		i++;
	}
	free(cmds);
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
		free_arr(cmd->args);
	if (cmd->path)
		free(cmd->path);
	free(cmd);
}

void	free_pipex(t_pipex *px)
{
	if (!px)
		return ;
	if (px->cmds)
		free_cmds(px->cmds);
	free(px);
}

void	free_arr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	err(char *err_text, t_pipex *px, int errnum)
{
	if (errnum == EFAULT || errnum == ENOENT)
	{
		write(2, "Command not found: ", 19);
		write(2, err_text, ft_strlen(err_text));
		write(2, "\n", 1);
	}
	else
		perror(err_text);
	safe_close(&px->fd.in);
	safe_close(&px->fd.out);
	safe_close(&px->fd.pipe[0]);
	safe_close(&px->fd.pipe[1]);
	safe_close(&px->fd.prev_read);
	free_pipex(px);
	if (errnum == ENOENT || errnum == EFAULT)
		exit (127);
	else if (errnum == EACCES)
		exit (126);
	else
		exit (1);
}
