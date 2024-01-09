/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:04:36 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/09 13:06:40 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	ft_is_valid_code(char *str)
{
	int	i;

	i = 0;
	if (ft_strlen(str) > 11)
		return (FALSE);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
		{
			if (str[i] == '-' && i == 0)
				i++;
			else
				return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

int	ft_exit(char **cmd_args, t_code *code, t_alloc **garbage)
{
	(void)garbage;
	if (cmd_args[1] != NULL)
	{
		if (ft_is_valid_code(cmd_args[1]))
		{
			if (cmd_args[2])
				return (code->code_status = 1, write(2, "minishell: exit: too many arguments\n", 37));
			else
			{
				code->code_status = ft_atoi(cmd_args[1]);
				// if (garbage)
				// 	free_garbage(garbage, 0);
				exit(code->code_status);
			}
		}
		else
		{
			write(2, "minishell: exit: numeric argument required\n", 44);
			code->code_status = 255; //testé sur macOS donc a verifier
			printf("code status = %d\n", code->code_status);
			// if (garbage)
			// 	free_garbage(garbage, 0);
			exit(code->code_status);
		}
	}
	else
		exit(code->code_status);
}
