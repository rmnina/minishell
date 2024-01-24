/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 19:16:31 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/24 19:24:39 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

t_alloc	*create_garbage_node(void *ptr, int cat)
{
	t_alloc	*new;

	new = NULL;
	new = (t_alloc *)malloc(sizeof(t_alloc));
	if (!new)
		return (NULL);
	new->adr = ptr;
	new->cat = cat;
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
}
