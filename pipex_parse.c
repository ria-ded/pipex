/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:31:56 by mdziadko          #+#    #+#             */
/*   Updated: 2025/04/17 14:59:48 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	find_executable(char **dirs, t_cmd *cmd)
{
	int		i;
	char	*temp;
	char	*full_path;

	i = 0;
	while (dirs[i])
	{
		temp = ft_strjoin(dirs[i], "/");
		if (!temp)
			return (0);
		full_path = ft_strjoin(temp, cmd->args[0]);
		free(temp);
		if (!full_path)
			return (0);
		if (access(full_path, X_OK) == 0)
		{
			cmd->path = full_path;
			return (1);
		}
		free (full_path);
		i++;
	}
	return (0);
}

int	find_path(t_cmd *cmd, char **envp)
{
	int		i;
	char	**dirs;

	i = 0;
	cmd->path = NULL;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			dirs = ft_split(envp[i] + 5, ':');
			if (!dirs)
				return (0);
			find_executable(dirs, cmd);
			free_arr(dirs);
			return (1);
		}
		i++;
	}
	return (1);
}

t_cmd	*extract_cmd(char *argv, char **envp)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = ft_split(argv, ' ');
	if (!cmd->args)
	{
		free(cmd);
		return (NULL);
	}
	cmd->path = NULL;
	find_path(cmd, envp);
	return (cmd);
}

t_cmd	**extract_cmds(char **argv, int count, char **envp)
{
	t_cmd	**cmds;
	int		i;

	cmds = malloc(sizeof(t_cmd *) * (count + 1));
	if (!cmds)
		return (NULL);
	i = 0;
	while (i < count)
	{
		cmds[i] = extract_cmd(argv[i + 2], envp);
		if (!cmds[i])
		{
			free_cmds(cmds);
			return (NULL);
		}
		i++;
	}
	cmds[count] = NULL;
	return (cmds);
}

int	parse_args(t_pipex *px, int argc, char **argv, char **envp)
{
	px->infile = argv[1];
	px->outfile = argv[argc - 1];
	px->cmds_count = argc - 3;
	px->envp = envp;
	px->cmds = extract_cmds(argv, px->cmds_count, envp);
	if (!px->cmds)
		return (1);
	return (0);
}
