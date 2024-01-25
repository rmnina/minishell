/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toolbox_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 23:39:25 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/24 23:39:38 by jdufour          ###   ########.fr       */
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
