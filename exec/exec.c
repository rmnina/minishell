/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/10 01:10:23 by jdufour          ###   ########.fr       */
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

void	execute_non_builtin(char **envp, t_code *code, char **cmd_args, t_alloc *garbage)
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
		execute_command(cmd_args, envp, garbage);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			code->code_status = WEXITSTATUS(status);
	}
}

void	heredoc_child(t_pipe *pipes, char **argv, char **envp, t_alloc *garbage)
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
	path = find_command_path(argv[0], garbage);
	if (!path)
	{
		perror("path");
		exit(EXIT_FAILURE);
	}
	new_argv[0] = ft_strdup(argv[0], garbage);
	new_argv[1] = NULL;
	execve(path, new_argv, envp);
	exit(EXIT_FAILURE);
}

int	ft_count(t_command *command, int *i)
{
	int	size;

	size = 0;
	while (command[*i + size].type && command[*i + size].type == WORD)
		size++;
	return (size);
}

char	**create_cmd_args(t_command *command, int *i, t_alloc *garbage)
{
	char	**cmd_args;
	int		j;

	j = 0;
	cmd_args = garb_malloc(sizeof(char *), ft_count(command, i) + 1, &garbage);
	if (!cmd_args)
		return (NULL);
	while (command[*i].type == WORD)
	{
		cmd_args[j] = ft_strjoin(cmd_args[j], command[*i].word, garbage);
		if (!cmd_args[j])
			return (NULL);
		*i += 1;
		j++;
	}
	return (cmd_args);
}

void	handle_command(char *input, t_code *code, char **envp, t_alloc *garbage)
{
	t_command	*command;
	char		**cmd_args;
	int			i;
	t_pipe		pipe;

	i = 0;
	command = ft_parsing(input, garbage);
	cmd_args = NULL;
	while (command[i].type != 0)
	{
		if (command[i].type == PIPE)
		{
			split_command_for_pipes(cmd_args, command, &pipe, &i, garbage); // a corriger
			execute_pipe(&pipe, envp, code, garbage);
		}
		if (command[i].type <= LEFT_CHEV && command[i].type >= DB_RIGHT_CHEV)
			handle_redirection(code, &i, command, envp, garbage);
		if (command[i].type == 0)
			break;
		else if (command[i].type == WORD)
		{
			cmd_args = create_cmd_args(command, &i, garbage);
			if (execute_builtins(cmd_args, envp, code) == -1)
				execute_non_builtin(envp, code, cmd_args, garbage);
		}
	}
}



