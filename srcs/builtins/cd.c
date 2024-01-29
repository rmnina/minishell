/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:31:57 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/29 14:34:25 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	replace_pwd(char *path, char **envp, t_alloc **garbage)
{
	int		i;

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
	else
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

int	ft_cd_main(t_minishell **main, t_alloc **garbage)
{
	char	*path;
	char	pwd[PATH_MAX];

	if ((*main)->cmd_args[2])
	{
		write(2, "minishell: cd: too many arguments\n", 35);
		return ((*main)->code_status = 1);
	}
	path = change_directory(main, (*main)->cmd_args[1], garbage);
	if (path == NULL)
	{
		perror("cd");
		return ((*main)->code_status = 1);
	}
	else
		replace_pwd(getcwd(pwd, sizeof(pwd)), (*main)->envp, garbage);
	return ((*main)->code_status = 0);
}

int	ft_cd(t_minishell **main, t_alloc **garbage)
{
	if ((*main)->last_cd_path == NULL)
		(*main)->last_cd_path = ft_g_strdup(ft_getenv(main, "PWD"), ENV, garbage);
	if ((*main)->cmd_args[1] == NULL \
	|| ft_strcmp((*main)->cmd_args[1], "~") == 0)
		return (cd_tilde(main, garbage));
	else if (ft_strcmp((*main)->cmd_args[1], "-") == 0)
		return (cd_hyphen(main, garbage));
	else if ((*main)->cmd_args[1][0] == '-' && (*main)->cmd_args[1][1])
	{
		printf("minishell: cd: -%c: invalid option\n", (*main)->cmd_args[1][1]);
		return ((*main)->code_status = 2);
	}
	else
		return (ft_cd_main(main, garbage));
}
