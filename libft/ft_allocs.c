/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_allocs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 19:16:31 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/14 01:09:46 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_alloc	*create_garbage_node(void *ptr)
{
	t_alloc	*new;

	new = (t_alloc *)malloc(sizeof(t_alloc));
	if (!new)
		return (NULL);
	new->adr = ptr;
	new->next = NULL;
	return (new);
}

t_alloc	*ft_garblast(t_alloc *last)
{
	if (!last)
		return (NULL);
	while (last->next)
		last = last->next;
	return (last);
}

void	add_garbage_node(t_alloc **garbage, t_alloc *new)
{
	t_alloc	*last;

	if (!new)
		return ;
	if (*garbage)
	{
		last = ft_garblast(*garbage);
		last->next = new;
	}
	else
		*garbage = new;
	// printf("node = %p, garbage = %p\n", new->adr, garbage);
}

void	*garb_malloc(size_t type, size_t size, t_alloc **garbage)
{
	void	*ptr;
	t_alloc	*new;

	ptr = NULL;
	if (size && type)
		ptr = malloc(type * size);
	if (!ptr)
		return (NULL);
	ft_memset(ptr, 0, (type * size));
	new = create_garbage_node(ptr);
	if (*garbage == NULL)
		(*garbage) = new;
	else
		add_garbage_node(garbage, new);
	return (ptr);
}

void	free_garbage(t_alloc **garbage, int i)
{
	t_alloc	*temp;

	temp = *garbage;
	if (!temp)
		return ;
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
	if (i == 1)
		printf("Error malloc : please try again\n");
}

