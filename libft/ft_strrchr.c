/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 22:55:37 by jdufour           #+#    #+#             */
/*   Updated: 2023/05/02 13:05:48 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	int				len;
	unsigned char	new_c;

	len = ft_strlen(str);
	new_c = (unsigned char)c;
	while (len > 0 && str[len] != new_c)
		len--;
	if (str[len] == new_c)
		return ((char *)str + len);
	else
		return (NULL);
}
