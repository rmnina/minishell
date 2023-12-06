/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:04:36 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/04 15:22:45 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_valid_number(const char *str, int *num)
{
	long	res;
	int		sign;

	res = 0;
	sign = 1;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (false);
		res = res * 10 + (*str - '0');
		if (res * sign > INT_MAX || res * sign < INT_MIN)
			return (false);
		str++;
	}
	*num = (int)(res * sign);
	return (true);
}

int	ft_exit(char **cmd_args, t_code *code)
{
	int	status;

	status = code->code_status;
	if (cmd_args[1] != NULL)
	{
		if (!is_valid_number(cmd_args[1], &status))
		{
			printf("exit: %s: numeric argument required\n", cmd_args[1]);
			status = 2;
		}
		status = status % 256;
	}
	else
		status = code->code_status;
	exit(status);
}

