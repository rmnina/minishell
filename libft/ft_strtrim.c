/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 23:31:41 by jdufour           #+#    #+#             */
/*   Updated: 2023/05/02 22:30:48 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_set(const char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static unsigned int	ft_first_set(const char *s1, const char *set)
{
	unsigned int	i;

	i = 0;
	while (ft_is_set(s1[i], set) && s1[i])
		i++;
	return (i);
}

static unsigned int	ft_last_set(const char *s1, const char *set)
{
	unsigned int	i;

	i = (ft_strlen(s1) - 1);
	while (ft_is_set(s1[i], set) && i > 0)
		i--;
	return (i);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	char	*res;
	size_t	i;

	if (!s1 || !set)
		return (NULL);
	if (*s1 == '\0')
		return (ft_strdup(""));
	i = (ft_last_set(s1, set) - ft_first_set(s1, set) + 1);
	res = ft_substr(s1, ft_first_set(s1, set), i);
	return (res);
}
