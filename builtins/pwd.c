/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 18:42:47 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/04 15:22:55 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(char **unused_args, char **unused_envp, t_code *code)
{
	char	*cwd;

	(void)unused_args;
	(void)unused_envp;
	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		printf("%s\n", cwd);
		free (cwd);
		code->code_status = 0;
	}
	else
	{
		perror("pwd");
		code->code_status = 1;
	}
	return (code->code_status);
}


