/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 10:21:29 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/29 16:50:50 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*find_command_in_segment(char *segment, char *command, \
t_alloc **garbage)
{
	char	full_path[PATH_MAX];

	if (segment == NULL || command == NULL \
		|| ft_strlen(segment) + ft_strlen(command) + 2 > PATH_MAX)
		return (NULL);
	ft_strcpy(full_path, segment);
	ft_strcat(full_path, "/");
	ft_strcat(full_path, command);
	if (access(full_path, X_OK) == 0)
		return (ft_g_strdup(full_path, EXEC, garbage));
	return (NULL);
}

char	*find_command_path(t_minishell **main, char *command, t_alloc **garbage)
{
	char	*path_env;
	char	*start;
	char	*end;
	char	*found_path;

	found_path = NULL;
	if (ft_strchr(command, '/') != NULL)
		return (ft_g_strdup(command, EXEC, garbage));
	path_env = ft_getenv(main, "PATH");
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
