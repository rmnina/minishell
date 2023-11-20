/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 11:35:43 by jdufour           #+#    #+#             */
/*   Updated: 2023/05/02 13:04:42 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*map;

	i = 0;
	map = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!(map))
		return (NULL);
	while (s[i])
	{
		map[i] = (*f)(i, s[i]);
		i++;
	}
	map[i] = '\0';
	return (map);
}
