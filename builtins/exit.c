/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:04:36 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/12 23:39:18 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// bool	is_valid_number(const char *str, int *num)
// {
// 	long	res;
// 	int		sign;

// 	res = 0;
// 	sign = 1;
// 	if (*str == '-' || *str == '+')
// 	{
// 		if (*str == '-')
// 			sign = -1;
// 		str++;
// 	}
// 	while (*str)
// 	{
// 		if (*str < '0' || *str > '9')
// 			return (false);
// 		res = res * 10 + (*str - '0');
// 		if (res * sign > INT_MAX || res * sign < INT_MIN)
// 			return (false);
// 		str++;
// 	}
// 	*num = (int)(res * sign);
// 	return (true);
// }

int	ft_exit(char **cmd_args, t_code *code, t_alloc *garbage)
{
	if (cmd_args[1] != NULL)
		code->code_status = ft_atoi(cmd_args[1]);
	free_garbage(&garbage, 1);
	exit(code->code_status);
}
