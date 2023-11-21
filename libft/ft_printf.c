/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 00:02:47 by jdufour           #+#    #+#             */
/*   Updated: 2023/09/24 18:48:15 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putadr(unsigned long n)
{
	char	*hex;
	int		size;

	hex = "0123456789abcdef";
	size = 0;
	if (!n)
	{
		size = ft_putstr(PTR_NULL);
		return (size);
	}
	if (n > 15)
		size += ft_putadr(n / 16);
	n = n % 16;
	size++;
	ft_putchar(hex[n]);
	return (size);
}

int	choose_format(char *format, va_list args)
{
	int				size;
	unsigned long	p;

	size = 0;
	if (*format == 'd' || *format == 'i')
		size = ft_putnbr(va_arg(args, int));
	if (*format == 'u')
		size = ft_putunsigned(va_arg(args, unsigned int));
	if (*format == 's')
		size = ft_putstr(va_arg(args, char *));
	if (*format == 'c')
		size = ft_putchar(va_arg(args, int));
	if (*format == '%')
		size = ft_putchar('%');
	if (*format == 'x' || *format == 'X')
		size = ft_put_hex(va_arg(args, unsigned int), *format);
	if (*format == 'p')
	{
		p = va_arg(args, unsigned long);
		if (p != 0)
			size = ft_putstr("0x");
		size += ft_putadr(p);
	}
	return (size);
}

int	ft_printf(char *format, ...)
{
	int		value;
	va_list	args;

	value = 0;
	va_start(args, format);
	if (!format)
		return (-1);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			value += choose_format(format++, args);
		}
		else
		{
			ft_putchar(*format);
			value++;
			format++;
		}
	}
	va_end(args);
	return (value);
}

// int	main ()
// {
// 	char	*buffer = NULL;
// 	printf("printf addr = %p\n", buffer);
// 	ft_printf("ftprintf addr = %p\n", buffer);	
// 	int i = ft_printf(NULL);
// 	ft_printf("%d\n", i);
// 	return (0);
// }