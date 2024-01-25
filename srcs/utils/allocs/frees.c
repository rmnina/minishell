/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 23:43:42 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/25 01:06:52 by jdufour          ###   ########.fr       */
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
	part_free_garb(garbage, EXEC);
}

t_alloc	*free_first_nodes(t_alloc **garbage, int cat)
{
	t_alloc	*temp;

	temp = *garbage;
	while (temp && temp->cat == cat)
	{
		*garbage = temp->next;
		temp->next = NULL;
		if (temp->adr)
		{
			free(temp->adr);
			temp->adr = NULL;
		}
		free(temp);
		temp = *garbage;
	}
	return (temp);
}

int	free_middle_nodes(void **pos, t_alloc **temp, t_alloc **prev, int cat)
{
	*pos = (*temp)->next;
	if ((*temp)->cat == cat)
	{
		(*temp)->next = NULL;
		if ((*temp)->adr)
		{
			free((*temp)->adr);
			(*temp)->adr = NULL;
		}
		free(*temp);
		(*prev)->next = *pos;
		return (1);
	}
	return (0);
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
