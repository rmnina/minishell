/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/30 18:27:05 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtins(char **cmd_args, char **envp)
{
	if (cmd_args[0] == NULL)
		return (0);
	if (ft_strcmp(cmd_args[0], "cd") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("cd"))
		return (my_cd(cmd_args));
	if (ft_strcmp(cmd_args[0], "echo") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("echo"))
		return (my_echo(cmd_args));
	if (ft_strcmp(cmd_args[0], "env") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("env"))
		return (my_env(envp));
	if (ft_strcmp(cmd_args[0], "exit") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("exit"))
		return (my_exit(cmd_args));
	if (ft_strcmp(cmd_args[0], "export") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("export"))
		return (my_export(cmd_args));
	if (ft_strcmp(cmd_args[0], "pwd") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("pwd"))
		return (my_pwd(NULL, NULL));
	if (ft_strcmp(cmd_args[0], "unset") == 0
		&& ft_strlen(cmd_args[0]) == ft_strlen("unset"))
		return (my_unset(&envp, cmd_args + 1));
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
	perror("execve");
	exit(EXIT_FAILURE);
}

void	handle_command(char *input, t_code *code, char **argv, char **envp)
{
	char		**cmd_args;
	t_command	exec;
	t_pipe		pipes;

	if (ft_strcmp(input, "$?") == 0)
	{
		execute_status_builtin(code);
		return ;
	}
	if (commands_with_pipes_detected(input))
	{
		split_command_for_pipes(input, &pipes);
		execute_pipe(&pipes, envp);
	}
	else
	{
		cmd_args = init_parsing(input);
		if (handle_redirection(&exec, input, argv, envp))
		{
			free_parsed_command_line(cmd_args);
			return ;
		}
		if (execute_builtins(cmd_args, envp) == -1)
			execute_non_builtin(input, envp, code);
	}
	free_parsed_command_line(cmd_args);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	pid_t	pid;
// 	int		status;

// 	(void)argc;
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		exit(EXIT_FAILURE);
// 	}
// 	else if (pid == 0)
// 		execute_command(argv[1], envp);
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 		if (WIFEXITED(status))
// 			WEXITSTATUS(status);
// 	}
// 	return (0);
// }
