/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/13 19:33:29 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_multipipes(t_command *command, t_alloc *garbage, char **envp, \
char **cmd_args, int *i, t_code *code)
{
	t_pipe	pipes;
	pid_t	pid;
	int		status;
	int		old_fd;

	status = 0;
	old_fd = -1;
	while (command[*i].type != 0)
	{
		cmd_args = create_cmd_args(command, i, garbage);
		if (command[*i].type == PIPE || *i > 0)
		{
			pipe(pipes.fd);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("pid");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (*i > 0 && old_fd != -1)
			{
				dup2(old_fd, STDIN_FILENO);
				close(old_fd);
			}
			if (command[*i].type == PIPE)
			{
				dup2(pipes.fd[1], STDOUT_FILENO);
				close(pipes.fd[1]);
			}
			close(pipes.fd[0]);
			if (execute_builtins(cmd_args, envp, code, garbage) == -1)
			{
				execute_non_builtin(envp, code, cmd_args, garbage);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if (*i > 0 && old_fd != -1)
			{
				close(old_fd);
				//close(pipes.fd[1]);
			}
			if (command[*i].type == PIPE)
			{
				old_fd = pipes.fd[0];
				close(pipes.fd[1]);
			}
			else
			{
				close(pipes.fd[1]);
			}
		}
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			code->code_status = WEXITSTATUS(status);
		}
		(*i)++;
	}
	if (old_fd != -1)
		close(old_fd);
}


// void	process_pipe(char **cmd_args, t_pipe *pipes, char **envp, t_alloc *garbage)
// {
// 	char	*path;

// 	path = find_command_path(cmd_args[0], garbage);
// 	dup2(pipes->pipefd[pipes->dup_fd], pipes->dup_fd);
// 	close(pipes->pipefd[0]);
// 	close(pipes->pipefd[1]);
// 	execve(path, cmd_args, envp);
// 	perror("execve");
// 	exit(EXIT_FAILURE);
// }

// int	launch_pipe(t_pipe *pipes, char **envp, t_alloc *garbage)
// {
// 	pid_t	pid1;
// 	pid_t	pid2;
// 	int		status1;
// 	int		status2;
// 	int		code_status;

// 	status1 = 0;
// 	status2 = 0;
// 	code_status = 0;
// 	pid1 = fork();
// 	if (pid1 == 0)
// 	{
// 		pipes->dup_fd = 1;
// 		process_pipe(pipes->command1, pipes, envp, garbage);
// 	}
// 	pid2 = fork();
// 	if (pid2 == 0)
// 	{
// 		pipes->dup_fd = 0;
// 		process_pipe(pipes->command2, pipes, envp, garbage);
// 	}
// 	close(pipes->pipefd[0]);
// 	close(pipes->pipefd[1]);
// 	waitpid(pid1, &status1, 0);
// 	waitpid(pid2, &status2, 0);
// 	if (WIFEXITED(status1))
// 		code_status = WEXITSTATUS(status1);
// 	if (WIFEXITED(status2) && WEXITSTATUS(status2) != 0)
// 		code_status = WEXITSTATUS(status2);
// 	return(code_status);
// }

// void	execute_pipe(t_pipe *pipes, char **envp, t_code *code, t_alloc *garbage)
// {
// 	int		pipe_status;

// 	pipe(pipes->pipefd);
// 	pipe_status = launch_pipe(pipes, envp, garbage);
// 	code->code_status = pipe_status;
// }

// pid_t	heredoc_pipe(t_pipe *pipes)
// {
// 	pid_t	pid;

// 	if (pipe(pipes->pipefd) == -1)
// 	{
// 		perror("pipe");
// 		exit(EXIT_FAILURE);
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	return (pid);
// }

// void	pid_redir(t_command *command, char **argv, char **envp, t_code *code, t_alloc *garbage)
// {
// 	pid_t	pid;
// 	int		status;

// 	status = 0;
// 	pid = fork();
// 	if (pid == 0)
// 		execute_redirection(command, argv, envp, garbage);
// 	waitpid(pid, &status, 0);
// 	if (WIFEXITED(status))
// 		code->code_status = WEXITSTATUS(status);
// }
