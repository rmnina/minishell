/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:06:16 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/22 14:48:36 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_echo(char **argv, t_code *code)
{
	int	i;
	int	line;

	i = 1;
	line = 1;
	if (argv[1] != NULL && ft_strcmp(argv[1], "-n") == 0)
	{
		line = 0;
		i++;
	}
	while (argv[i])
	{
		if (argv[i] && ft_strcmp(argv[i], "$?") == 0)
		{
			if (!code->code_status)
				printf("0");
			else
				printf("%d", code->code_status);
		}
		else
			printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (line)
		printf("\n");
	code->code_status = 0;
	return (0);
}

