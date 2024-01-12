/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:31:57 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/11 12:11:22 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	change_directory(t_minishell **main, char *path)
{
	char	*new_path;
	
	if (path == NULL)
		return (false);
	new_path = realpath(path, NULL);
	if (new_path == NULL)
		return (false);
	if (chdir(new_path) != 0)
	{
		free(new_path);
		return (false);
	}
	free((*main)->last_cd_path);
	(*main)->last_cd_path = (*main)->cd_path;
	(*main)->cd_path = new_path;
	return (true);
}

int	cd_hyphen(t_minishell **main)
{
	if ((*main)->last_cd_path == NULL)
	{
		printf("cd: OLDPWD not set\n");
		(*main)->code_status = 1;
		return ((*main)->code_status);
	}
	printf("%s\n", (*main)->last_cd_path);
	if (!change_directory(main, (*main)->last_cd_path))
	{
		perror("cd");
		(*main)->code_status = 1;
		return ((*main)->code_status);
	}
	return ((*main)->code_status);
}

int	cd_tilde(t_minishell **main)
{
	char	*home_path;
	
	home_path = ft_getenv(main, "HOME");
	if (!change_directory(main, home_path))
	{
		perror("cd");
		(*main)->code_status = 1;
	}
	return ((*main)->code_status);
}

int	ft_cd(t_minishell **main)
{
	if ((*main)->cmd_args[1] == NULL || ft_strcmp((*main)->cmd_args[1], "~") == 0)
		return (cd_tilde(main));
	else if (ft_strcmp((*main)->cmd_args[1], "-") == 0)
		return (cd_hyphen(main));
	else
	{
		if (!change_directory(main, (*main)->cmd_args[1]))
		{
			perror("cd");
			(*main)->code_status = 1;
			return ((*main)->code_status);
		}
	}
	return ((*main)->code_status);
}

