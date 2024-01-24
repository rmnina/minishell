/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:04:36 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 11:31:24 by juandrie         ###   ########.fr       */
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

int	exit_non_numeric_code(t_minishell **main, t_alloc **garbage)
{
	write(2, "exit\nminishell: exit: numeric argument required\n", 49);
	(*main)->code_status = 2;
	if (garbage)
		free_garbage(garbage);
	exit((*main)->code_status);
}

int	exit_with_code(t_minishell **main, t_alloc **garbage)
{
	(*main)->code_status = ft_atoi((*main)->cmd_args[1]);
	if (garbage)
		free_garbage(garbage);
	printf("exit\n");
	exit((*main)->code_status);
}

int	exit_no_code(t_minishell **main, t_alloc **garbage)
{
	printf("exit\n");
	if (garbage)
		free_garbage(garbage);
	exit((*main)->code_status);
}

int	ft_exit(t_minishell **main, t_alloc **garbage)
{
	if ((*main)->cmd_args[1] != NULL)
	{
		if (ft_is_valid_code((*main)->cmd_args[1]))
		{
			if ((*main)->cmd_args[2])
				return ((*main)->code_status = 1, \
				write(2, "minishell: exit: too many arguments\n", 37));
			else
				exit_with_code(main, garbage);
		}
		else
			exit_non_numeric_code(main, garbage);
	}
	else
		exit_no_code(main, garbage);
	return ((*main)->code_status);
}
