/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toolbox_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 23:39:25 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/29 17:13:41 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_error(t_minishell **main, char *str, int i)
{
	ft_putstr_fd(str, 2);
	(*main)->code_status = i;
	return (-1);
}

int	is_only_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] == 32)
		i++;
	if (!str[i])
		return (1);
	return (0);
}

void	error_path(t_minishell **main, t_alloc **garbage)
{
	if ((*main)->cmd_args[0])
	{
		ft_putstr_fd((*main)->cmd_args[0], 2);
		ft_putstr_fd(": ", 2);
	}
	free_garbage(garbage);
	perror("command not found");
	exit(127);
}
