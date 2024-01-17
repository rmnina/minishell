/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:31:57 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/17 17:15:16 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	replace_pwd(char *path, char **envp, t_alloc **garbage)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp("PWD", envp[i], 3) != 0)
		i++;
	envp[i] = ft_strjoin("PWD=\0", path, garbage);
	if (!envp[i])
		return ;
}

char	*change_directory(t_minishell **main, char *path)
{
	char	*new_path;

	if (path == NULL)
		return (false);
	new_path = realpath(path, NULL);
	if (new_path == NULL)
		return (NULL);
	if (chdir(new_path) != 0)
	{
		free(new_path);
		return (NULL);
	}
	(*main)->last_cd_path = (*main)->cd_path;
	(*main)->cd_path = new_path;
	free(new_path);
	return (path);
}

int	cd_hyphen(t_minishell **main)
{
	if ((*main)->last_cd_path == NULL)
	{
		printf("cd: OLDPWD not set\n");
		return ((*main)->code_status = 1);
	}
	printf("%s\n", (*main)->last_cd_path);
	if (!change_directory(main, (*main)->last_cd_path))
	{
		perror("cd");
		return ((*main)->code_status = 1);
	}
	return ((*main)->code_status = 0);
}

int	cd_tilde(t_minishell **main)
{
	char	*home_path;

	home_path = ft_getenv(main, "HOME");
	if (!change_directory(main, home_path))
	{
		perror("cd");
		return ((*main)->code_status = 1);
	}
	return ((*main)->code_status = 0);
}

int	ft_cd(t_minishell **main, t_alloc **garbage)
{
	char	*path;

	path = NULL;
	if ((*main)->cmd_args[1] == NULL || \
	ft_strcmp((*main)->cmd_args[1], "~") == 0)
		return (cd_tilde(main));
	else if (ft_strcmp((*main)->cmd_args[1], "-") == 0)
		return (cd_hyphen(main));
	else
	{
		if ((*main)->cmd_args[2])
		{
			printf("minishell : cd : too many arguments");
			return ((*main)->code_status = 1);
		}
		if ((path = change_directory(main, (*main)->cmd_args[1])) == NULL)
		{
			perror("cd");
			return ((*main)->code_status = 1);
		}
		else
			replace_pwd(path, (*main)->envp, garbage);
	}
	return ((*main)->code_status = 0);
}

