/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 22:37:08 by jdufour           #+#    #+#             */
/*   Updated: 2023/05/02 22:05:02 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_int_size(long int n)
{
	int	size;

	size = 0;
	if (n < 0)
	{
		size++;
		n *= -1;
	}	
	if (n == 0)
		size = 1;
	while (n > 0)
	{
		size++;
		n = n / 10;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	long int	new_n;
	int			size;
	char		*res;

	new_n = (long int) n;
	size = ft_int_size(new_n) - 1;
	res = ft_calloc((size + 2), sizeof(char));
	if (!(res))
		return (NULL);
	if (new_n < 0)
	{
		new_n *= -1;
		res[0] = '-';
	}
	if (new_n == 0)
		res[0] = '0';
	while (new_n > 0)
	{
		res[size] = (new_n % 10) + '0';
		new_n /= 10;
		size--;
	}
	return (res);
}
/*
int main()
{
    printf("%s\n", ft_itoa(42));
}
*/
