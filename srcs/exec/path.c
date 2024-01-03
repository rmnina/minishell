/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:21:29 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/02 20:57:32 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*find_command_in_segment(char *segment, t_main_items *main, t_alloc *garbage)
{
	char	full_path[PATH_MAX];

	if (segment == NULL || main->command == NULL \
		|| ft_strlen(segment) + ft_strlen(main->command) + 2 > PATH_MAX)
		return (NULL);
	ft_strcpy(full_path, segment);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, main->command);
	if (access(full_path, X_OK) == 0)
		return (ft_strdup(full_path, garbage));
	return (NULL);
}

char	*find_command_path(t_main_items *main, t_alloc *garbage)
{
	char	*path_env;
	char	*start;
	char	*end;
	char	*found_path;

	found_path = NULL;
	if (ft_strchr(main->command, '/') != NULL)
		return (ft_strdup(main->command, garbage));
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	start = path_env;
	end = ft_strchr(start, ':');
	while (end != NULL)
	{
		*end = '\0';
		found_path = find_command_in_segment(start, main->command, garbage);
		*end = ':';
		if (found_path != NULL)
			return (found_path);
		start = end + 1;
		end = ft_strchr(start, ':');
	}
	return (find_command_in_segment(start, main->command, garbage));
}

void	execute_command(t_main_items *main, t_alloc *garbage)
{
	char	*path;

	path = NULL;
	if (!main->cmd_args)
	{
		perror("Error creating command args");
		exit(EXIT_FAILURE);
	}
	path = find_command_path(main->cmd_args[0], garbage);
	if (!path)
	{
		perror("Command not found");
		exit(127);
	}
	execve(path, main->cmd_args, main->envp);
	perror("execve");
	exit(EXIT_FAILURE);
}