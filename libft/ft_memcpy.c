/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 20:49:42 by jdufour           #+#    #+#             */
/*   Updated: 2023/05/02 22:05:12 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t size)
{
	size_t		i;
	char		*new_dest;
	const char	*new_src;

	i = 0;
	new_dest = (char *)dest;
	new_src = (const char *)src;
	while (i < size)
	{
		new_dest[i] = new_src[i];
		i++;
	}
	return (dest);
}
/*
int main()
{
    int array[] = { 1, 2, 3 };
    int *cpy = NULL;
    size_t size = sizeof(int) * 3;
    cpy = malloc(size);
    cpy = ft_memcpy(cpy, array, size);
    for (size_t i = 0; i < 3; i++)
        printf("%d\n", cpy[i]);
    free (cpy);
}

int main()
{
    char str[] = "salut";
    char *cpy = NULL;
    size_t size = sizeof(char) * 6;
    cpy = malloc(size);
    cpy = ft_memcpy(cpy, str, size);
    printf("%s\n", cpy);
    free (cpy);
}
*/
