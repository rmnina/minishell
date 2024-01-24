/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:31:57 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 11:30:50 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	replace_pwd(char *path, char **envp, t_alloc **garbage)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp("PWD", envp[i], 3) != 0)
		i++;
	envp[i] = ft_g_strjoin("PWD=\0", path, ENV, garbage);
	if (!envp[i])
		return ;
}

int	cd_hyphen(t_minishell **main, t_alloc **garbage)
{
	if ((*main)->last_cd_path == NULL)
	{
		printf("cd: OLDPWD not set\n");
		return ((*main)->code_status = 1);
	}
	printf("%s\n", (*main)->last_cd_path);
	if (!change_directory(main, (*main)->last_cd_path, garbage))
	{
		perror("cd");
		return ((*main)->code_status = 1);
	}
	return ((*main)->code_status = 0);
}

int	cd_tilde(t_minishell **main, t_alloc **garbage)
{
	char	*home_path;

	home_path = ft_getenv(main, "HOME");
	if (!change_directory(main, home_path, garbage))
	{
		perror("cd");
		return ((*main)->code_status = 1);
	}
	return ((*main)->code_status = 0);
}

int	ft_cd(t_minishell **main, t_alloc **garbage)
{
	if ((*main)->last_cd_path == NULL)
		(*main)->last_cd_path = getcwd(NULL, 0);
	if ((*main)->cmd_args[1] == NULL \
	|| ft_strcmp((*main)->cmd_args[1], "~") == 0)
		return (cd_tilde(main, garbage));
	else if (ft_strcmp((*main)->cmd_args[1], "-") == 0)
		return (cd_hyphen(main, garbage));
	else
		return (ft_cd_main(main, garbage));
}

int	ft_cd_main(t_minishell **main, t_alloc **garbage)
{
	char	*path;

	if ((*main)->cmd_args[2])
	{
		printf("minishell: cd: too many arguments\n");
		return ((*main)->code_status = 1);
	}
	path = change_directory(main, (*main)->cmd_args[1], garbage);
	if (path == NULL)
	{
		perror("cd");
		return ((*main)->code_status = 1);
	}
	else
		replace_pwd(path, (*main)->envp, garbage);
	return ((*main)->code_status = 0);
}
