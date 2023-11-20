/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_format.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 19:07:32 by jdufour           #+#    #+#             */
/*   Updated: 2023/07/12 20:07:17 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
	{
		ft_putstr("(null)");
		return (6);
	}
	while (s[i])
	{
		write(1, &s[i], 1);
		i++;
	}
	return (i);
}

int	ft_putchar(char c)
{
	write(1, &c, 1);
	return (1);
}

int	ft_putnbr(int n)
{
	int	size;

	size = 0;
	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		return (11);
	}
	if (n < 0)
	{
		n *= -1;
		write(1, "-", 1);
		size++;
	}
	if (n >= 0 && n < 10)
		size += ft_putchar(n + '0');
	else if (n > 9)
	{
		size += ft_putnbr(n / 10);
		size += ft_putnbr(n % 10);
	}
	return (size);
}

int	ft_putunsigned(unsigned int n)
{
	int	size;

	size = 0;
	if (n < 10)
		size += ft_putchar(n + '0');
	else if (n > 9)
	{
		size += ft_putunsigned(n / 10);
		size += ft_putunsigned(n % 10);
	}	
	return (size);
}

int	ft_put_hex(unsigned int n, char c)
{
	char	*hexmin;
	char	*hexmaj;
	int		size;

	hexmin = "0123456789abcdef";
	hexmaj = "0123456789ABCDEF";
	size = 0;
	if (n < 16)
	{
		if (c == 'x')
			size += ft_putchar(hexmin[n]);
		else if (c == 'X')
			size += ft_putchar(hexmaj[n]);
	}
	else if (n > 15)
	{
		size += ft_put_hex(n / 16, c);
		size += ft_put_hex(n % 16, c);
	}
	return (size);
}
