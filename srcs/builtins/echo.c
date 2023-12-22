/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:06:16 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/22 14:55:45 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	code_status(char *str, t_code *code)
{
	if (str && ft_strcmp(str, "$?") == 0)
	{
		if (!code->code_status)
			printf("0");
		else
			printf("%d", code->code_status);
		return (1);
	}
	return (0);
}

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
		if (!code_status(argv[i], code))
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

