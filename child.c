/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/20 18:59:34 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>
#include <stdlib.h>

char	*build_and_check_path(char *segment, char *command)
{
	char	full_path[PATH_MAX];

	if (ft_strlen(segment) + ft_strlen(command) + 2 > PATH_MAX) 
		return (NULL);
	ft_strcpy(full_path, segment);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, command);
	if (access(full_path, X_OK) == 0)
		return (ft_strdup(full_path));
	return (NULL);
}

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
	while ((end = ft_strchr(start, ':')) != NULL)
	{
		*end = '\0';
		found_path = find_command_in_segment(start, command);
		*end = ':';
		if (found_path != NULL)
			return (found_path);
		start = end + 1;
	}
	return (find_command_in_segment(start, command));
}

int main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	*command;

	(void)argc;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		command = "pwd";
		path = find_command_path(command);
		if (!path)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		execve(path, argv, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			WEXITSTATUS(status);
	}
	return (0);
}
