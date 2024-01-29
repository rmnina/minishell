/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 17:07:39 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/29 16:38:42 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(char *src, size_t n)
{
	char	*dest;
	size_t	len;

	len = 0;
	while (src[len] && len < n)
		len++;
	dest = malloc(sizeof(char) * (len + 1));
	if (!(dest))
		return (NULL);
	dest[len] = '\0';
	while (len--)
	{
		dest[len] = src[len];
	}
	return (dest);
}
