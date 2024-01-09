/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_allocs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 19:16:31 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/09 00:56:48 by jdufour          ###   ########.fr       */
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
		ptr = malloc(type * size + 1);
	if (!ptr)
		return (NULL);
	ft_memset(ptr, 0, type * size + 1);
	new = create_garbage_node(ptr);
	if (*garbage == NULL)
		*garbage = new;
	else
		add_garbage_node(garbage, new);
	return (ptr);
}

void	free_garbage(t_alloc **garbage, int i)
{
	void	*pos;

	pos = garbage;
	// printf("pos = %p, garb = %p \n", pos, garbage);
	if (!garbage)
		return ;
	while (pos != NULL)
	{
		pos = (*garbage)->next;
		(*garbage)->next = NULL;
		// printf("pos = %p, garb = %p \n", pos, (*garbage)->next);
		if ((*garbage)->adr != NULL)
		{
			free((*garbage)->adr);
			(*garbage)->adr = NULL;
		}
		if (garbage != NULL)
			free(garbage);
		garbage = pos;
	}
	garbage = NULL;
	if (i == 1)
		printf("Error malloc : please try again\n");
}

// int main(void)
// {
// 	t_alloc *garbage = NULL;
// 	char *str = garb_malloc(sizeof(char), 5, &garbage);
// 	int *array = garb_malloc(sizeof(int), 5, &garbage);
// 	str[0] = 's';
// 	str[1] = 'a';
// 	str[2] = 'l';
// 	str[3] = 'u';
// 	array[0] = 6;
// 	array[1] = 1;
// 	array[2] = 2;
// 	array[3] = 3;
// 	if (str)
// 		printf("malloc 1 success\n");
// 	if (array)
// 		printf("malloc 2 success\n");
// 	if (str)
// 		printf("str 1 = %c\n", str[0]);
// 	if (array)
// 		printf("array 1 = %d\n", array[0]);
// 	free_garbage(&garbage);
// }