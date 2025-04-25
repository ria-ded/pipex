/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdziadko <mdziadko@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:25:38 by mdziadko          #+#    #+#             */
/*   Updated: 2025/04/25 17:42:10 by mdziadko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <string.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <sys/wait.h>
# include "libft/libft.h"
# include "printf/ft_printf.h"

// STRUCTURES

typedef struct s_cmd
{
	char	*path;
	char	**args;
}			t_cmd;

typedef struct s_fd
{
	int	in;
	int	out;
	int	prev_read;
	int	pipe[2];
}		t_fd;

typedef struct s_pipex
{
	char	*infile;
	char	*outfile;
	t_cmd	**cmds;
	int		cmds_count;
	char	**envp;
	t_fd	fd;
	pid_t	*pids;
}			t_pipex;

// PIPEX EXECUTION
void	run_fst_cmd(t_pipex *px, int i);
void	run_mid_cmd(t_pipex *px, int i);
void	run_last_cmd(t_pipex *px, int i);
void	run_pipe(t_pipex *px);
int		execute_pipex(t_pipex *px);

// PIPEX UTILS
t_pipex	*pipex_init(int argc, char **argv, char **envp);
int		wait_for_children(t_pipex *px);
void	safe_close(int *fd);
void	safe_dup2(int old_fd, int new_fd, t_pipex *px);

// PIPEX PARSING

int		find_executable(char **dirs, t_cmd *cmd);
int		find_path(t_cmd *cmd, char **envp);
t_cmd	*extract_cmd(char *argv, char **envp);
t_cmd	**extract_cmds(char **argv, int count, char **envp);
int		parse_args(t_pipex *px, int argc, char **argv, char **envp);

// PIPEX CLEANUP
void	free_cmd(t_cmd *cmd);
void	free_cmds(t_cmd **cmds);
void	free_pipex(t_pipex *px);
void	free_arr(char **arr);
void	err(char *err_text, t_pipex *px, int errnum);

#endif