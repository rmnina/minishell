/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 22:50:05 by jdufour           #+#    #+#             */
/*   Updated: 2023/05/02 13:01:04 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	unsigned char	new_c;

	new_c = (unsigned char)c;
	while (*str && *str != new_c)
		str++;
	if (*str == new_c)
		return ((char *)str);
	else
		return (NULL);
}
