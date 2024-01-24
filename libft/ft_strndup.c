/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 17:07:39 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/09 00:02:02 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(char *src, size_t n, t_alloc **garbage)
{
	char	*dest;
	size_t	len;

	len = 0;
	while (src[len] && len < n)
		len++;
	dest = garb_malloc(sizeof(char), (len + 1), garbage);
	if (!(dest))
		return (NULL);
	dest[len] = '\0';
	while (len--)
	{
		dest[len] = src[len];
	}
	return (dest);
}
