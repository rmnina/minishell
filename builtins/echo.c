/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:06:16 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/22 17:01:35 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	my_echo(char **argv)
{
	int	i;
	int	line;

	i = 1;
	line = 1;
	if (argv[1] != NULL && strcmp(argv[1], "-n") == 0)
	{
		line = 0;
		i++;
	}
	while (argv[i] != NULL)
	{
		printf("%s", argv[i]);
		if (argv[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (line)
		printf("\n");
	return (0);
}

// int	main(int ac, char **av)
// {
// 	if (ac > 1)
// 		my_echo(av);
// 	else
// 		printf("\n");
// 	return (0);
// }
