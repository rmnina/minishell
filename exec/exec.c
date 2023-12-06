/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/06 15:30:31 by juandrie         ###   ########.fr       */
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

void	execute_non_builtin(char **envp, t_code *code, char **cmd_args)
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
		execute_command(cmd_args, envp);
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

char	**create_cmd_args(t_command *command)
{
	int		word_count;
	int		i;
	char	**cmd_args;

	word_count = 0;
	i = 0;
	while (command[word_count].type != 0)
		word_count++;
	cmd_args = malloc(sizeof(char *) * word_count);
	if (!cmd_args)
		exit(EXIT_FAILURE);
	while (i < word_count)
	{
		cmd_args[i] = ft_strdup(command[i].word);
		if (cmd_args[i] == NULL)
			exit(EXIT_FAILURE);
		i++;
	}
	cmd_args[i] = NULL;
	return (cmd_args);
}
void	handle_command(char *input, t_code *code, char **argv, char **envp)
{
	t_command	*command;
	t_quotes	quotes = {FALSE, FALSE, FALSE};
	t_expand	expand = {0, 0, FALSE};
	t_pipe		pipes;
	char		**cmd_args;


	command = get_command(input, &quotes, &expand);
	ft_error_lexer(command);
	if (ft_strcmp(input, "$?") == 0)
	{
		execute_status_builtin(code);
		return ;
	}
	else if (command->type == WORD)
	{
		cmd_args = create_cmd_args(command);
		if (execute_builtins(cmd_args, envp, code) == -1)
			execute_non_builtin(envp, code, cmd_args);
		free_parsed_command_line(cmd_args);
	}
	if (command->type == PIPE)
	{
		split_command_for_pipes(input, &pipes);
		execute_pipe(&pipes, envp, code);
	}
	else if (command->type == LEFT_CHEV || \
		command->type == RIGHT_CHEV || command->type == DB_LEFT_CHEV \
		|| command->type == DB_RIGHT_CHEV)
	{
		handle_redirection(code, command, argv, envp);
		return ;
	}
	//free_parsed_command_line(cmd_args);
	ft_free_command(command);
}
