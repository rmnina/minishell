/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 18:10:49 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/22 18:17:43 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(const char *haystack, const char *needle)
{
	const char	*p;
	const char	*begin;
	const char	*pattern;

	p = haystack;
	if (!*needle)
		return ((char *)haystack);
	while (*p != '\0')
	{
		if (*p == *needle)
		{
			begin = p;
			pattern = needle;
			while (*pattern != '\0' && *p == *pattern)
			{
				++p;
				++pattern;
			}
			if (*pattern == '\0')
				return ((char *)begin);
			p = begin;
		}
		++p;
	}
	return (NULL);
}
