/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 11:36:05 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/09 23:06:51 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len, t_alloc *garbage)
{
	char	*substr;
	size_t	i;

	i = 0;
	if (len > (unsigned int)ft_strlen(s))
		len = (unsigned int)ft_strlen(s);
	if (start + len > (unsigned int)ft_strlen(s))
		len = len - start;
	if (start > (unsigned int)ft_strlen(s))
		len = 0;
	substr = garb_malloc(sizeof(char), len + 1, &garbage);
	if (!(substr))
		return (NULL);
	ft_memset(substr, 0, (len + 1) * sizeof(char));
	while (i < len && s[start])
	{
		substr[i] = s[start];
		i++;
		start++;
	}
	return (substr);
}
