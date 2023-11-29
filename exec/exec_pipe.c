/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/29 22:12:05 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_pipe(char **cmd_args, t_pipe *pipes, char **argv, char **envp)
{
	char	*path;

	(void)argv;
	path = find_command_path(cmd_args[0]);
	dup2(pipes->pipefd[pipes->dup_fd], pipes->dup_fd);
	close(pipes->pipefd[0]);
	close(pipes->pipefd[1]);
	execve(path, cmd_args, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}


void	prepare_pipe_execution(t_pipe *pipes, char ***argv1, char ***argv2)
{
	*argv1 = init_parsing(pipes->command1);
	*argv2 = init_parsing(pipes->command2);
	pipe(pipes->pipefd);
}

void	launch_pipe(t_pipe *pipes, char **argv1, char **argv2, char **envp)
{
	pid_t	pid1;
	pid_t	pid2;

	pid1 = fork();
	if (pid1 == 0)
	{
		pipes->dup_fd = 1;
		process_pipe(argv1, pipes, argv2, envp);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		pipes->dup_fd = 0;
		process_pipe(argv2, pipes, argv2, envp);
	}
	close(pipes->pipefd[0]);
	close(pipes->pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	free_parsed_command_line(argv1);
	free_parsed_command_line(argv2);
}

void	execute_pipe(t_pipe *pipes, char **envp)
{
	char	**argv1;
	char	**argv2;

	prepare_pipe_execution(pipes, &argv1, &argv2);
	launch_pipe(pipes, argv1, argv2, envp);
}

pid_t	heredoc_pipe(t_pipe *pipes)
{
	pid_t	pid;

	if (pipe(pipes->pipefd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	pid_redir(t_exec *exec, char **argv, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		execute_redirection(exec, argv, envp);
	waitpid(pid, NULL, 0);
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
//     execute_pipe(&pipes, argv_for_ls, argv_for_grep, envp);

//     return (0);
// }
