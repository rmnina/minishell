/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:06:16 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/11 16:30:29 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	are_flags_n(char **cmd_args, int *i)
{
	int	j;

	j = 0;
	if (ft_strncmp(cmd_args[*i], "-n", 2) == 0)
	{
		// printf("cmd_args = %s \n", cmd_args[*i]);
		while (ft_strncmp(cmd_args[*i], "-n", 2) == 0)
		{
			j = 2;
			while (cmd_args[*i][j] == 'n')
				j++;
			if (!cmd_args[*i][j])
				*i += 1;
			else if (*i == 1 && cmd_args[*i][j] != 'n')
				return (0);
			else
				break ;
		}
		return (1);
	}
	return (0);
}

int	code_status(t_minishell **main, char *str)
{
	if (str && ft_strcmp(str, "$?") == 0)
	{
		if (!(*main)->code_status)
			printf("0");
		else
			printf("%d", (*main)->code_status);
		return (1);
	}
	return (0);
}

int	ft_echo(t_minishell **main)
{
	int	i;
	int	line;

	i = 1;
	line = 1;
	// for (int j = 0; (*main)->cmd_args[j]; j++)
	// {
	// 	dprintf(2, "cmd args echo = %s\n", (*main)->cmd_args[j]);
	// }
	if ((*main)->cmd_args[i] != NULL && are_flags_n((*main)->cmd_args, &i))
		line = 0;
	while ((*main)->cmd_args[i])
	{
		if (!code_status(main, (*main)->cmd_args[i]))
			printf("%s", (*main)->cmd_args[i]);
		if ((*main)->cmd_args[i + 1])
			printf(" ");
		i++;
	}
	if (line)
		printf("\n");
	(*main)->code_status = 0;
	return (0);
}

