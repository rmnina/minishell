/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/06 15:05:59 by juandrie         ###   ########.fr       */
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
	t_quotes	quotes = {FALSE, FALSE, FALSE};
	t_expand	expand = {0, 0, FALSE};
	t_command	*command1;
	t_command	*command2;

	command1 = get_command(pipes->command1, &quotes, &expand);
	command2 = get_command(pipes->command2, &quotes, &expand);

	*argv1 = create_cmd_args(command1);
	*argv2 = create_cmd_args(command2);
	pipe(pipes->pipefd);
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
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status1))
		code_status = WEXITSTATUS(status1);
	if (WIFEXITED(status2) && WEXITSTATUS(status2) != 0)
		code_status = WEXITSTATUS(status2);
	free_parsed_command_line(argv1);
	free_parsed_command_line(argv2);
	return(code_status);
}

void	execute_pipe(t_pipe *pipes, char **envp, t_code *code)
{
	char	**argv1;
	char	**argv2;
	int		pipe_status;

	prepare_pipe_execution(pipes, &argv1, &argv2);
	pipe_status = launch_pipe(pipes, argv1, argv2, envp);
	code->code_status = pipe_status;
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
	pid = fork();
	if (pid == 0)
		execute_redirection(command, argv, envp);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		code->code_status = WEXITSTATUS(status);
}

