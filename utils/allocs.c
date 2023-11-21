/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 00:44:08 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/19 02:35:32 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_to_garbage(void *ptr, t_alloc *garbage)
{
	garbage->adr[garbage->count] = ptr;
	garbage->count++; 
}

void	free_garbage(t_alloc *garbage)
{
	int	i;

	i = 0;
	while (i < garbage->count)
	{
		free(garbage->adr[i]);
		garbage->adr[i] = NULL;
		i++;
	}
	free(garbage);
}