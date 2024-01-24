/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 23:43:42 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/24 18:02:28 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	free_adr(t_alloc **garbage, void *adr)
{
	t_alloc	*temp;

	if (!(*garbage))
		return ;
	temp = *garbage;
	while (temp->next && temp->adr != adr)
	{
		*garbage = temp->next;
		if (temp->adr == adr)
		{
			free(temp->adr);
			temp->adr = NULL;
			free(temp);
		}
		temp = *garbage;
	}
}

void	free_small_garb(t_alloc **garbage)
{
	part_free_garb(garbage, PARSING);
	part_free_garb(garbage, PARSING);
}

void	part_free_garb(t_alloc **garbage, int cat)
{
	t_alloc	*temp;

	if (!(*garbage))
		return ;
	temp = *garbage;
	while (temp != NULL)
	{
		*garbage = temp->next;
		if (temp->cat == cat)
		{
			temp->next = NULL;
			if (temp->adr)
			{
				free(temp->adr);
				temp->adr = NULL;
			}
			free(temp);
		}
		temp = *garbage;
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
