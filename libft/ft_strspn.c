/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strspn.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 14:10:48 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/25 14:13:41 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strspn(const char *s1, const char *s2)
{
	const char	*p1;
	const char	*p2;

	p1 = s1;
	while (*p1)
	{
		p2 = s2;
		while (*p2)
		{
			if (*p1 == *p2)
				break ;
			p2++;
		}
		if (!*p2)
			break ;
		p1++;
	}
	return (p1 - s1);
}
