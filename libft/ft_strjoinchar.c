/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoinchar.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 01:57:10 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/19 01:57:25 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_char(char *s1, const char s2)
{
	int		size;
	char	*res;
	int		i;

	i = 0;
	size = 0;
	while (s1 && s1[size])
		size++;
	res = malloc(sizeof(char) * (size + 2));
	if (!(res))
		return (NULL);
	while (s1 && s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	res[i++] = s2;
	res[i] = '\0';
	free(s1);
	return (res);
}