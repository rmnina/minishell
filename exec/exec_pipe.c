/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/07 17:25:48 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	process_pipe(char **cmd_args, t_pipe *pipes, char **envp)
{
	char	*path;

	path = find_command_path(cmd_args[0]);
	// printf("Chemin de la commande: %s\n", path ? path : "Introuvable"); 
	dup2(pipes->pipefd[pipes->dup_fd], pipes->dup_fd);
	close(pipes->pipefd[0]);
	close(pipes->pipefd[1]);
	execve(path, cmd_args, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

int	launch_pipe(t_pipe *pipes, char **argv1, char **argv2, char **envp)
{
	pid_t	pid1;
	pid_t	pid2;
	int		status1;
	int		status2;
	int		code_status;

	status1 = 0;
	status2 = 0;
	code_status = 0;
	printf("Lancement de la première commande du pipe\n"); // Débogage
	pid1 = fork();
	if (pid1 == 0)
	{
		pipes->dup_fd = 1;
		process_pipe(argv1, pipes, argv2, envp);
	}
	printf("Lancement de la deuxième commande du pipe\n"); 
	pid2 = fork();
	if (pid2 == 0)
	{
		pipes->dup_fd = 0;
		process_pipe(argv2, pipes, argv1, envp);
	}
	close(pipes->pipefd[0]);
	close(pipes->pipefd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status1))
		code_status = WEXITSTATUS(status1);
	if (WIFEXITED(status2) && WEXITSTATUS(status2) != 0)
		code_status = WEXITSTATUS(status2);
	printf("Statut de sortie: %d\n", code_status);
	free_parsed_command_line(argv1);
	free_parsed_command_line(argv2);
	return(code_status);
}

void	execute_pipe(t_pipe *pipes, char **envp, t_code *code)
{
	char	**argv1;
	char	**argv2;
	int		pipe_status;

	printf("execute_pipe started\n");
	pipe(pipes->pipefd);
	pipe_status = launch_pipe(pipes, argv1, argv2, envp);
	code->code_status = pipe_status;
	printf("execute_pipe ended with status: %d\n", pipe_status);
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

void	pid_redir(t_command *command, char **argv, char **envp, t_code *code)
{
	pid_t	pid;
	int		status;

	status = 0;
	printf("Creating child process for redirection\n"); // Débogage
	pid = fork();
	if (pid == 0)
		execute_redirection(command, argv, envp);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		code->code_status = WEXITSTATUS(status);
	printf("Child process finished with status: %d\n", status); 
}

