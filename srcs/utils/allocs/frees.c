/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 23:43:42 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/24 19:26:34 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	free_adr(t_alloc **garbage, void *adr)
{
	if ((*garbage)->adr == adr)
	{	
		free(adr);
		(*garbage)->adr = NULL;
	}
	while ((*garbage)->adr != adr)
		(*garbage) = (*garbage)->next;
	free(adr);
	(*garbage)->adr = NULL;
}

void	free_small_garb(t_alloc **garbage)
{
	part_free_garb(garbage, PARSING);
	part_free_garb(garbage, EXEC);
}

void	part_free_garb(t_alloc **garbage, int cat)
{
	if ((*garbage)->cat && (*garbage)->cat == cat)
	{
		if ((*garbage)->adr)
			free((*garbage)->adr);
		(*garbage)->adr = NULL;
	}
	while (*garbage)
	{
		if ((*garbage)->cat == cat)
		{
			if ((*garbage)->adr)
				free((*garbage)->adr);
			(*garbage)->adr = NULL;
		}
		*garbage = (*garbage)->next;
	}
}

void	free_garbage(t_alloc **garbage)
{
	t_alloc	*temp;

	if (!(*garbage))
		return ;
	temp = *garbage;
	while (temp != NULL)
	{
		*garbage = temp->next;
		temp->next = NULL;
		if (temp->adr != NULL)
		{
			free(temp->adr);
			temp->adr = NULL;
		}
		free(temp);
		temp = *garbage;
	}
	*garbage = NULL;
	garbage = NULL;
}
