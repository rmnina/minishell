/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:21:29 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/06 18:41:00 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	execute_command(char **cmd_args, char **envp)
{
	char		*path;

	if (!cmd_args)
	{
		perror("Error creating command args");
		exit(EXIT_FAILURE);
	}
	path = find_command_path(cmd_args[0]);
	printf("path dans exec command: %s\n", cmd_args[0]);
	if (!path)
	{
		perror("Command not found");
		exit(127);
	}
	execve(path, cmd_args, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}
