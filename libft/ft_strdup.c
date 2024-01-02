/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 11:35:08 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/28 17:08:20 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(char *src, t_alloc *garbage)
{
	int		i;
	char	*dest;

	i = 0;
	dest = garb_malloc(sizeof(char), (ft_strlen(src) + 1), &garbage);
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
