/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 23:43:42 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/28 23:37:12 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	free_adr(void *adr, t_alloc **garbage)
{
	t_alloc	*temp;
	t_alloc	*prev;
	void	*pos;
	
	if (!(*garbage))
		return ;
	temp = *garbage;
	pos = *garbage;
	prev = NULL;
	if (free_first_adr(garbage, adr))
		return ;
	while (temp->next && temp->adr != adr)
	{
		if (free_middle_adr(&pos, &temp, &prev, adr))
			return ;
		else
			prev = temp;
		temp = pos;
	}
}

void	part_free_garb(t_alloc **garbage, int cat)
{
	t_alloc	*temp;
	void	*pos;
	t_alloc	*prev;

	if (!(*garbage))
		return ;
	pos = *garbage;
	temp = *garbage;
	prev = temp;
	if (temp->cat == cat)
	{
		temp = free_first_nodes(garbage, cat);
		pos = *garbage;
		prev = temp;
	}
	while (temp)
	{
		if (!free_middle_nodes(&pos, &temp, &prev, cat))
			prev = temp;
		temp = pos;
	}
}

void	free_small_garb(t_alloc **garbage)
{
	part_free_garb(garbage, PARSING);
	part_free_garb(garbage, EXEC);
}

void	free_garbage(t_alloc **garbage)
{
	free_small_garb(garbage);
	part_free_garb(garbage, ENV);
}
