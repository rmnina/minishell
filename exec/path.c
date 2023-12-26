/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:21:29 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/22 12:41:43 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_command_in_segment(char *segment, char *command, t_alloc *garbage)
{
	char	full_path[PATH_MAX];

	if (segment == NULL || command == NULL \
		|| ft_strlen(segment) + ft_strlen(command) + 2 > PATH_MAX)
		return (NULL);
	ft_strcpy(full_path, segment);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, command);
	if (access(full_path, X_OK) == 0)
		return (ft_strdup(full_path, garbage));
	return (NULL);
}

char	*find_command_path(char *command, t_alloc *garbage)
{
	char	*path_env;
	char	*start;
	char	*end;
	char	*found_path;

	found_path = NULL;
	if (ft_strchr(command, '/') != NULL)
		return (ft_strdup(command, garbage));
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	start = path_env;
	end = ft_strchr(start, ':');
	while (end != NULL)
	{
		*end = '\0';
		found_path = find_command_in_segment(start, command, garbage);
		*end = ':';
		if (found_path != NULL)
			return (found_path);
		start = end + 1;
		end = ft_strchr(start, ':');
	}
	return (find_command_in_segment(start, command, garbage));
}

void	execute_command(char **cmd_args, char **envp, t_alloc *garbage)
{
	char	*path;
	
	path = NULL;
	if (!cmd_args)
	{
		perror("Error creating command args");
		exit(EXIT_FAILURE);
	}
	path = find_command_path(cmd_args[0], garbage);
	if (!path)
	{
		perror("Command not found");
		exit(127);
	}
	execve(path, cmd_args, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}
