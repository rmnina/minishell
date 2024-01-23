/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:39:20 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/23 19:55:51 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	resolve_path(const char *path, char *resolved)
{
	if (path == NULL || resolved == NULL)
	{
		errno = EINVAL;
		return (-1);
	}
	if (path[0] == '/')
	{
		ft_strncpy(resolved, path, PATH_MAX - 1);
		resolved[PATH_MAX - 1] = '\0';
	}
	else
	{
		if (getcwd(resolved, PATH_MAX) == NULL)
			return (-1);
		ft_strncat(resolved, "/", PATH_MAX - ft_strlen(resolved) - 1);
		ft_strncat(resolved, path, PATH_MAX - ft_strlen(resolved) - 1);
	}
	return (0);
}

char	*ft_realpath(char *path, char *resolved_path, t_alloc **garbage)
{
	char	temp[PATH_MAX];
	char	*final_path;

	final_path = NULL;
	if (resolve_path(path, temp) == -1)
		return (NULL);
	if (resolved_path == NULL)
	{
		final_path = (char *)garb_malloc(sizeof(char), \
		ft_strlen(temp) + 1, garbage);
		if (final_path == NULL)
			return (NULL);
		ft_strcpy(final_path, temp);
	}
	else
	{
		final_path = resolved_path;
		ft_strncpy(final_path, temp, PATH_MAX);
	}
	return (final_path);
}

char	*change_directory(t_minishell **main, char *path, t_alloc **garbage)
{
	char	*new_path;

	if (path == NULL)
	{
		printf("change_directory: path is NULL\n");
		return (false);
	}
	new_path = ft_realpath(path, NULL, garbage);
	if (new_path == NULL)
		return (NULL);
	if (chdir(new_path) != 0)
		return (NULL);
	if ((*main)->cd_path != NULL)
		(*main)->last_cd_path = ft_strdup((*main)->cd_path, garbage);
	(*main)->cd_path = ft_strdup(new_path, garbage);
	return (path);
}
