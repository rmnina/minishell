/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 18:53:46 by jdufour           #+#    #+#             */
/*   Updated: 2023/05/02 13:00:14 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//typedef unsigned int size_t;
//#include <stdio.h>
//#include <string.h>

#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t size)
{
	size_t	i;
	char	*new_ptr;

	i = 0;
	new_ptr = ptr;
	while (i < size)
	{
		new_ptr[i] = value;
		i++;
	}
	return (ptr);
}
/*
int main()
{
    int tab[] = {1, 2, 3};
    char    array[5] = "salut";
    
    ft_memset(tab, 50, 3);
    for (int i = 0; i < 3; i++)
        printf("%d\n", tab[i]);
}
*/
