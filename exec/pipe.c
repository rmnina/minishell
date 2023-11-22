/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/22 12:51:39 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_pipe(char *command, t_pipe pipes, char **argv, char **envp)
{
	char	*path;

	path = find_command_path(command);
	dup2(pipes.pipefd[pipes.dup_fd], pipes.dup_fd);
	close(pipes.pipefd[0]);
	close(pipes.pipefd[1]);
	execve(path, argv, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	execute_pipe(t_pipe pipes, char **argv1, char **argv2, char **envp)
{
	pid_t	pid1;
	pid_t	pid2;

	pipe(pipes.pipefd);
	pid1 = fork();
	if (pid1 == 0)
	{
		pipes.dup_fd = 1;
		process_pipe(pipes.command1, pipes, argv1, envp);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		pipes.dup_fd = 0;
		process_pipe(pipes.command2, pipes, argv2, envp);
	}
	close(pipes.pipefd[0]);
	close(pipes.pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}


// int	main(int argc, char **argv, char **envp)
// {
// 	t_pipe pipes;
// 	pipes.command1 = "ls";
// 	pipes.command2 = "grep";
// 	char	*argv_for_ls[] = {"ls", NULL};
// 	char	*argv_for_grep[] = {"grep", "pipe", NULL};
// 	(void)argc;
// 	(void)argv;
// 	//execute_pipe(command1, command2, argv_for_ls, envp);
//     execute_pipe(pipes, argv_for_ls, argv_for_grep, envp);

//     return (0);
// }
