/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:04:36 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/22 14:56:22 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_exit(char **cmd_args, t_code *code, t_alloc *garbage)
{
	if (cmd_args[1] != NULL)
		code->code_status = ft_atoi(cmd_args[1]);
	if (garbage)
		free_garbage(&garbage, 0);
	exit(code->code_status);
}
