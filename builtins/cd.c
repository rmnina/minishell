/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:31:57 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/21 11:23:09 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(const char *path)
{
	char	cwd[PATH_MAX];

	if (path == NULL || strcmp(path, "") == 0)
	{
		path = getenv("HOME");
		if (path == NULL)
			return (-1);
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return (-1);
	}
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("cd: getcwd");
		return (-1);
	}
	return (0);
}

// int main(int argc, char **argv) 
// {
//     if (argc == 1) 
//         cd("");
//     else if (argc > 1)
//         cd(argv[2]);
//     return 0;
// }

int	main(int argc, char **argv)
{
	if (argc == 1)
	{
		if (ft_cd("") != 0)
			printf("cd: failed to change to home directory\n");
	}
	else if (argc == 2)
	{
		if (ft_cd(argv[1]) != 0)
			printf("cd: failed to change directory to %s\n", argv[1]);
	}
	else
	{
		fprintf(stderr, "usage: cd [directory]\n");
		return (1);
	}
	return (0);
}
