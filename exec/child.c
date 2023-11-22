/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/22 19:35:02 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_command_in_segment(char *segment, char *command)
{
	char	full_path[PATH_MAX];

	if (segment == NULL || command == NULL \
		|| ft_strlen(segment) + ft_strlen(command) + 2 > PATH_MAX)
		return (NULL);
	ft_strcpy(full_path, segment);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, command);
	if (access(full_path, X_OK) == 0)
		return (ft_strdup(full_path));
	return (NULL);
}

char	*find_command_path(char *command)
{
	char	*path_env;
	char	*start;
	char	*end;
	char	*found_path;

	if (ft_strchr(command, '/') != NULL)
		return (ft_strdup(command));
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	start = path_env;
	end = ft_strchr(start, ':');
	while (end != NULL)
	{
		*end = '\0';
		found_path = find_command_in_segment(start, command);
		*end = ':';
		if (found_path != NULL)
			return (found_path);
		start = end + 1;
		end = ft_strchr(start, ':');
	}
	return (find_command_in_segment(start, command));
}

void	execute_command(char *input, char **envp)
{
	char	*path;
	char	**cmd_args;

	cmd_args = parse_command_line(input);
	if (!cmd_args)
	{
		perror("parse_commande_line");
		exit(EXIT_FAILURE);
	}
	path = find_command_path(cmd_args[0]);
	if (!path)
	{
		perror("Command not found");
		exit(EXIT_FAILURE);
	}
	execve(path, cmd_args, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

int	execute_builtins(char **cmd_args, char **envp)
{
	if (cmd_args[0] == NULL)
		return (0);
	if (strcmp(cmd_args[0], "cd") == 0)
		return (my_cd(cmd_args));
	if (strcmp(cmd_args[0], "echo") == 0)
		return (my_echo(cmd_args));
	if (strcmp(cmd_args[0], "env") == 0)
		return (my_env(envp));
	if (strcmp(cmd_args[0], "exit") == 0)
		return (my_exit(cmd_args));
	if (strcmp(cmd_args[0], "export") == 0)
		return (my_export(cmd_args));
	if (strcmp(cmd_args[0], "pwd") == 0)
		return (my_pwd(NULL, NULL));
	if (strcmp(cmd_args[0], "unset") == 0)
		return (my_unset(&envp, cmd_args + 1));
	return (-1);
}

void	handle_command(char *input, t_code *code, char **envp)
{
	pid_t	pid;
	int		status;
	char	**cmd_args;

	if (strcmp(input, "$?") == 0)
	{
		execute_status_builtin(code);
		return ;
	}
	cmd_args = parse_command_line(input);
	if (execute_builtins(cmd_args, envp) == -1)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
			execute_command(input, envp);
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				code->code_status = WEXITSTATUS(status);
		}
	}
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
