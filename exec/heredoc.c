/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/23 17:58:39 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	here_doc(const char *delimiter, t_pipe *pipes, char **argv, char **envp)
{
	pid_t	pid;
	char	*line;
	char	*path;

	if (pipe(pipes->pipefd) == -1)
		exit(EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
	{
		close(pipes->pipefd[1]);
		dup2(pipes->pipefd[0], STDIN_FILENO);
		close(pipes->pipefd[0]);
		path = find_command_path(argv[0]);
		if (!path)
			exit(EXIT_FAILURE);
		execve(path, argv, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		close(pipes->pipefd[0]);
		line = readline(NULL);
		while (line != NULL)
		{
			if (strcmp(line, delimiter) == 0)
			{
				free(line);
				break ;
			}
			write(pipes->pipefd[1], line, ft_strlen(line));
			write(pipes->pipefd[1], "\n", 1);
			free(line);
			line = readline(NULL);
		}
		if (line != NULL)
			free(line);
		close(pipes->pipefd[1]);
		waitpid(pid, NULL, 0);
	}
}
