/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 11:20:46 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/11 07:40:47 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_env(t_minishell **main)
{
	int	i;

	i = 0;
	while ((*main)->envp[i] != NULL)
	{
		printf("%s\n", (*main)->envp[i]);
		i++;
	}
	(*main)->code_status = 0;
	return (0);
}

