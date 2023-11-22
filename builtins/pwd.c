/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 18:42:47 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/22 17:34:38 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	my_pwd(char **unused_args, char **unused_envp)
{
	char	*cwd;

	(void)unused_args;
	(void)unused_envp;
	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		printf("%s\n", cwd);
		free (cwd);
	}
	else
	{
		perror("pwd");
	}
	return (0);
}

// int	main(int ac, char **av)
// {
// 	(void)ac;
// 	(void)av;
// 	pwd();
// }
