/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toolbox_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 23:39:25 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/28 21:50:56 by jdufour          ###   ########.fr       */
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
