/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:31:54 by mdziadko          #+#    #+#             */
/*   Updated: 2025/04/14 17:03:36 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
		err("Open infile", px, errno);
	px->fd.out = open(px->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (px->fd.out == -1)
		err("Open outfile", px, errno);
	px->fd.prev_read = -1;
	run_pipe(px);
	close(px->fd.in);
	close(px->fd.out);
	return (wait_for_children(px->last_pid));
}
