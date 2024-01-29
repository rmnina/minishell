/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 16:39:20 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/29 00:48:40 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_realpath(char *path, t_alloc **garbage) 
{
    char *resolved;
	
    resolved = NULL;
    resolved = garb_malloc(sizeof(char), PATH_MAX, ENV, garbage);
    if (!resolved) 
	{
        perror("malloc");
        return (NULL);
    }
    if (path[0] && path[0] == '/') 
	{
        resolved = path;
        return (resolved);
    }
    if (getcwd(resolved, PATH_MAX) == NULL) 
	{
        perror("cwd");
        return NULL;
    }
    resolved = ft_g_strjoin(resolved, "/", ENV, garbage);
	resolved = ft_g_strjoin(resolved, path, ENV, garbage);
    return (resolved);
}

char	*change_directory(t_minishell **main, char *path, t_alloc **garbage)
{
	char	*new_path;

	if (path == NULL)
	{
		printf("cd: path is NULL\n");
		return (false);
	}
	new_path = ft_realpath(path, garbage);
	if (new_path == NULL)
		return (NULL);
	if (chdir(new_path) != 0)
		return (NULL);
	if ((*main)->cd_path != NULL)
		(*main)->last_cd_path = (*main)->cd_path;
	(*main)->cd_path = new_path;
	return (new_path);
}
