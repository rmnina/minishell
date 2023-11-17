/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 21:50:14 by jdufour           #+#    #+#             */
/*   Updated: 2023/05/02 12:24:52 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t size)
{
	size_t		i;
	size_t		count;
	char		*new_dest;
	const char	*new_src;

	i = 0;
	count = size - 1;
	new_dest = dest;
	new_src = src;
	while (i < size && count + 1 > i)
	{
		if (new_dest > new_src)
		{
			new_dest[count] = new_src[count];
			count--;
		}
		else
		{
			new_dest[i] = new_src[i];
			i++;
		}
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
    cpy = ft_memmove(cpy, array, size);
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
    cpy = ft_memmove(cpy, str, size);
    printf("%s\n", cpy);
    free (cpy);
}
*/
