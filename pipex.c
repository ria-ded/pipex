/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:43:53 by mdziadko          #+#    #+#             */
/*   Updated: 2025/04/09 15:29:21 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*px;

	if (argc < 5)
		return (0);
	px = parse_args(argc, argv, envp);
	if (!px)
		return (1);
	execute_pipex(px);
	free_pipex(px);
}

/*
	printf("infile: %s\n", px->infile);
	printf("outfile: %s\n", px->outfile);
	printf("cmd_count: %i\n", px->cmds_count);

	for (int i = 0; px->cmds[i]; i++)
	{
		printf("cmd[%i]: %s\n", i, px->cmds[i]->path);
		for (int j = 0; px->cmds[i]->args[j]; j++)
			printf("	arg[%i]: %s\n", j, px->cmds[i]->args[j]);
	}
	for (int i = 0; px->envp[i]; i++)
		printf("envp[%i]: %s\n", i, px->envp[i]);
*/
