/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toolbox_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 23:39:25 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/25 17:20:19 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_error(t_minishell **main, char *str, int i)
{
	ft_putstr_fd(str, 2);
	write(2, "\n", 1);
	(*main)->code_status = i;
	return (-1);
}
int	is_only_spaces(const char *str)
{
	if (!str || *str == '\0')
		return (1);
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
			return (0);
		str++;
	}
	return (1);
}
