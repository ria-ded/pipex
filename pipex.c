/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:43:53 by mdziadko          #+#    #+#             */
/*   Updated: 2025/04/25 19:06:13 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	*px;
	int		exit_code;

	if (argc < 5)
		return (EXIT_FAILURE);
	px = pipex_init(argc, argv, envp);
	if (!px)
		return (EXIT_FAILURE);
	exit_code = execute_pipex(px);
	free_pipex(px);
	return (exit_code);
}
