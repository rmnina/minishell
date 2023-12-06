/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/06 13:47:24 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtins(char **cmd_args, char **envp, t_code *code)
{
	if (cmd_args[0] == NULL)
		return (0);
	if (ft_strcmp(cmd_args[0], "cd") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("cd"))
		return (ft_cd(cmd_args, code));
	if (ft_strcmp(cmd_args[0], "echo") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("echo"))
		return (ft_echo(cmd_args, code));
	if (ft_strcmp(cmd_args[0], "env") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("env"))
		return (ft_env(envp, code));
	if (ft_strcmp(cmd_args[0], "exit") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("exit"))
		return (ft_exit(cmd_args, code));
	if (ft_strcmp(cmd_args[0], "export") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("export"))
		return (ft_export(envp, code));
	if (ft_strcmp(cmd_args[0], "pwd") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("pwd"))
		return (ft_pwd(NULL, NULL, code));
	if (ft_strcmp(cmd_args[0], "unset") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("unset"))
		return (ft_unset(&envp, cmd_args + 1, code));
	return (-1);
}

void	execute_non_builtin(char *input, char **envp, t_code *code)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror ("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		execute_command(input, envp);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			code->code_status = WEXITSTATUS(status);
	}
}

void	heredoc_child(t_pipe *pipes, char **argv, char **envp)
{
	char	*path;
	char	*new_argv[2];

	close(pipes->pipefd[1]);
	if (dup2(pipes->pipefd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	close(pipes->pipefd[0]);
	path = find_command_path(argv[0]);
	if (!path)
	{
		perror("path");
		exit(EXIT_FAILURE);
	}
	new_argv[0] = ft_strdup(argv[0]);
	new_argv[1] = NULL;
	execve(path, new_argv, envp);
	exit(EXIT_FAILURE);
}

// void	handle_command(char *input, t_code *code, char **argv, char **envp)
// {
// 	char		**cmd_args;
// 	t_pipe		pipes;

// 	if (ft_strcmp(input, "$?") == 0)
// 	{
// 		execute_status_builtin(code);
// 		return ;
// 	}
// 	if (commands_with_pipes_detected(input))
// 	{
// 		split_command_for_pipes(input, &pipes);
// 		execute_pipe(&pipes, envp, code);
// 	}
// 	else
// 	{
// 		cmd_args = init_parsing(input);
// 		if (handle_redirection(code, input, argv, envp))
// 		{
// 			free_parsed_command_line(cmd_args);
// 			return ;
// 		}
// 		if (execute_builtins(cmd_args, envp, code) == -1)
// 			execute_non_builtin(input, envp, code);
// 		free_parsed_command_line(cmd_args);
// 	}
// }


void	handle_command(char *input, t_code *code, char **argv, char **envp)
{
	t_command	*command;
	t_quotes	quotes = {FALSE, FALSE, FALSE};
	t_expand	expand = {0, 0, FALSE};
	t_pipe		pipes;
	int			i;
	char		**cmd_args;

	cmd_args = NULL;
	command = get_command(input, &quotes, &expand);
	ft_error_lexer(command);
	i = 0;
	if (ft_strcmp(input, "$?") == 0)
	{
		execute_status_builtin(code);
		return ;
	}
	if (command[i].type == WORD)
	{
		cmd_args = init_parsing(input);
		if (execute_builtins(cmd_args, envp, code) == -1)
			execute_non_builtin(input, envp, code);
	}
	else if (command[i].type == PIPE)
	{
		split_command_for_pipes(input, &pipes);
		execute_pipe(&pipes, envp, code);
		return ;
	}
	else if (command[i].type == LEFT_CHEV || \
		command[i].type == RIGHT_CHEV || command[i].type == DB_LEFT_CHEV \
		|| command[i].type == DB_RIGHT_CHEV)
	{
		//cmd_args = init_parsing(input);
		handle_redirection(code, &command[i], argv, envp);
		free_parsed_command_line(cmd_args);
	}
	else if (command[i].type == PIPE)
	{
		split_command_for_pipes(input, &pipes);
		execute_pipe(&pipes, envp, code);
		return ;
	}
	else if (command[i].type == LEFT_CHEV || \
		command[i].type == RIGHT_CHEV || command[i].type == DB_LEFT_CHEV \
		|| command[i].type == DB_RIGHT_CHEV)
	{
		//cmd_args = init_parsing(input);
		handle_redirection(code, &command[i], argv, envp);
		free_parsed_command_line(cmd_args);
	}
	else if (command[i].type == PIPE)
	{
		split_command_for_pipes(input, &pipes);
		execute_pipe(&pipes, envp, code);
		return ;
	}
	else if (command[i].type == LEFT_CHEV || \
		command[i].type == RIGHT_CHEV || command[i].type == DB_LEFT_CHEV \
		|| command[i].type == DB_RIGHT_CHEV)
	{
		//cmd_args = init_parsing(input);
		handle_redirection(code, &command[i], argv, envp);
		free_parsed_command_line(cmd_args);
		return ;
	}
	ft_free_command(command);
}
