/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 17:07:39 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/28 17:12:21 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(char *src, size_t n, t_alloc *garbage)
{
	char	*dest;
	size_t	len;

	len = ft_strlen(src);
	if (n < len)
		len = n;
	dest = garb_malloc(sizeof(char), (ft_strlen(src) + 1), &garbage);
	if (!(dest))
		return (NULL);
	dest[len] = '\0';
	while (len > 0)
	{
		dest[len] = src[len];
		len--;
	}
	return (dest);
}
