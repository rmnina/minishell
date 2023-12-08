/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinchar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 01:57:10 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/08 21:12:47 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_char(char *s1, const char c, t_alloc *garbage)
{
	int		size;
	char	*res;
	int		i;

	i = 0;
	size = 0;
	if (s1)
	{
		while (s1 && s1[size])
			size++;
	}
	res = garb_malloc(sizeof(char), size + 2, &garbage);
	if (!(res))
		return (NULL);
	if (size == 0)
	{
		res[size] = c;
		res[size + 1] = '\0';
		return (res);
	}
	while (s1 && s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	res[i++] = c;
	res[i] = '\0';
	free(s1);
	return (res);
}
