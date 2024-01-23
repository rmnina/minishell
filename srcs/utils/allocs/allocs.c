/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 17:07:39 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/23 23:43:24 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

char	*ft_g_strndup(char *src, size_t n, int cat, t_alloc **garbage)
{
	char	*dest;
	size_t	len;

	len = 0;
	while (src[len] && len < n)
		len++;
	dest = garb_malloc(sizeof(char), (len + 1), cat, garbage);
	if (!(dest))
		return (NULL);
	dest[len] = '\0';
	while (len--)
	{
		dest[len] = src[len];
	}
	return (dest);
}

char	*ft_g_strdup(char *src, int cat, t_alloc **garbage)
{
	int		i;
	char	*dest;

	i = 0;
	dest = garb_malloc(sizeof(char), (ft_strlen(src) + 1), cat, garbage);
	if (!(dest))
		return (NULL);
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	*garb_malloc(size_t type, size_t size, int cat, t_alloc **garbage)
{
	void	*ptr;
	t_alloc	*new;

	ptr = NULL;
	if (size && type)
		ptr = malloc(type * size);
	if (!ptr)
		return (NULL);
	ft_memset(ptr, 0, (type * size));
	new = create_garbage_node(ptr, cat);
	if (*garbage == NULL)
		(*garbage) = new;
	else
		add_garbage_node(garbage, new);
	return (ptr);
}