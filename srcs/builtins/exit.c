/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:04:36 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/08 15:35:49 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_exit(char **cmd_args, t_code *code, t_alloc *garbage)
{
	if (cmd_args[1] != NULL)
		code->code_status = ft_atoi(cmd_args[1]);
	else
		code->code_status = SPECIAL_EXIT_CODE;
	if (garbage)
		free_garbage(&garbage, 0);
	//printf("ft_exit called, exiting with code: %d\n", code->code_status);
	exit(code->code_status);
}
