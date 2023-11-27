/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/27 18:46:10 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	here_doc(const char *delimiter, t_pipe *pipes, char **argv, char **envp)
{
	pid_t	pid;
	char	*line;
	char	*path;

	printf("here_doc: Started with delimiter '%s'\n", delimiter);
	if (pipe(pipes->pipefd) == -1)
		exit(EXIT_FAILURE);
	printf("here_doc: Pipe created with file descriptors %d, %d\n", pipes->pipefd[0], pipes->pipefd[1]);
	pid = fork();
	printf("Forked process with PID: %d\n", pid);
	if (pid == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
	{
		printf("here_doc: Child process, executing command\n");
		close(pipes->pipefd[1]);
		dup2(pipes->pipefd[0], STDIN_FILENO);
		close(pipes->pipefd[0]);
		path = find_command_path(argv[0]);
		if (!path)
		{
			perror("find command path");
			exit(EXIT_FAILURE);
		}
		printf("here_doc: Executing command '%s'\n", argv[0]);
		for (int i = 0; argv[i] != NULL; i++) 
		{
    	printf("argv[%d] = '%s'\n", i, argv[i]);
		}
		execve(path, argv, envp);
		printf("Forked process with PID: %d\n", pid);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
    	printf("here_doc: Parent process, PID=%d\n", pid);
		close(pipes->pipefd[0]);
		line = readline(NULL);
		while (line != NULL)
		{
			if (ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				break ;
			}
			write(pipes->pipefd[1], line, ft_strlen(line));
			write(pipes->pipefd[1], "\n", 1);
			printf("here_doc : Read line: %s\n", line);
			free(line);
			line = readline(NULL);
		}
		close(pipes->pipefd[1]);
		waitpid(pid, NULL, 0);
	}
}
