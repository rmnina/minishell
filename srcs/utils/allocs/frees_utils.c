/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 23:29:05 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/29 17:13:23 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	free_first_adr(t_alloc **garbage, void *adr)
{
	t_alloc	*temp;

	temp = *garbage;
	if (temp->adr == adr)
	{
		*garbage = temp->next;
		temp->next = NULL;
		free(temp->adr);
		temp->adr = NULL;
		free(temp);
		return (1);
	}
	return (0);
}

int	free_middle_adr(void **pos, t_alloc **temp, t_alloc **prev, void *adr)
{
	*pos = (*temp)->next;
	if ((*temp)->adr == adr)
	{
		(*temp)->next = NULL;
		free((*temp)->adr);
		(*temp)->adr = NULL;
		free(*temp);
		(*prev)->next = *pos;
		return (1);
	}
	return (0);
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
