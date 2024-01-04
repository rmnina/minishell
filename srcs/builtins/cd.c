/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:31:57 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/04 18:07:34 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	change_directory(const char *path, t_code *code)
{
	char	*new_current;

	if (path == NULL)
		return (false);
	new_current = realpath(path, NULL);
	if (new_current == NULL)
		return (false);
	if (chdir(new_current) != 0)
	{
		free(new_current);
		return (false);
	}
	free(code->last);
	code->last = code->current;
	code->current = new_current;
	return (true);
}

int	ft_cd(char **args, t_code *code)
{
	char	*home_path;

	if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
	{
		home_path = getenv("HOME");
		if (!change_directory(home_path, code))
		{
			perror("cd");
			code->code_status = 1;
			return (code->code_status);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		if (code->last == NULL)
		{
			printf("cd: OLDPWD not set\n");
			code->code_status = 1;
			return (code->code_status);
		}
		printf("%s\n", code->last);
		if (!change_directory(code->last, code))
		{
			perror("cd");
			code->code_status = 1;
			return (code->code_status);
		}
	}
	else
	{
		if (!change_directory(args[1], code))
		{
			perror("cd");
			code->code_status = 1;
			return (code->code_status);
		}
	}
	return (code->code_status);
}

