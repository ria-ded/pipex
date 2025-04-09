/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:31:51 by mdziadko          #+#    #+#             */
/*   Updated: 2025/04/09 15:31:24 by mdziadko         ###   ########.fr       */
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
	int	i;

	if (!px)
		return ;
	i = 0;
	while (i < px->cmds_count)
	{
		free_cmd(px->cmds[i]);
		i++;
	}
	free(px->cmds);
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

void	err(char *err_text, t_pipex *px)
{
	perror(err_text);
	free_pipex(px);
	exit (1);
}
