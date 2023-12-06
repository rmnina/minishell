/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:31:57 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/05 18:01:09 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	my_cd(const char *path)
// {
// 	char	cwd[PATH_MAX];

// 	if (path == NULL || strcmp(path, "") == 0)
// 	{
// 		path = getenv("HOME");
// 		if (path == NULL)
// 			return (-1);
// 	}
// 	if (chdir(path) != 0)
// 	{
// 		perror("cd");
// 		return (-1);
// 	}
// 	if (getcwd(cwd, sizeof(cwd)) == NULL)
// 	{
// 		perror("cd: getcwd");
// 		return (-1);
// 	}
// 	return (0);
// }

int	ft_cd(char **args, t_code *code)
{
	const char	*path;

	if (args[1] == NULL)
	{
		path = getenv("HOME");
		if (path == NULL)
		{
			code->code_status = 1;
			return (code->code_status);
		}
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		code->code_status = 1;
		return (code->code_status);
	}
	return (code->code_status);
}


