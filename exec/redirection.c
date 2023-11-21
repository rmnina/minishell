/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 17:13:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/21 19:12:40 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redir(t_exec exec, char **argv, char **envp)
{
	int		fd;
	pid_t	pid;
	char	*path;

	pid = fork();
	if (pid == 0)
	{
		if (exec.redirect_type == REDIRECT_OUTPUT)
		{
			fd = open(exec.file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd, STDOUT_FILENO);
		}
		else if (exec.redirect_type == REDIRECT_INPUT)
		{
			fd = open(exec.file, O_RDONLY);
			dup2(fd, STDIN_FILENO);
		}
		close(fd);
		path = find_command_path(exec.command);
		execve(path, argv, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	waitpid(pid, NULL, 0);
}
