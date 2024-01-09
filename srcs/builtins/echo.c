/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:06:16 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/09 01:32:00 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	are_flags_n(char **argv, int *i)
{
	int	j;

	j = 0;
	if (ft_strncmp(argv[*i], "-n", 2) == 0)
	{
		// printf("argv = %s \n", argv[*i]);
		while (ft_strncmp(argv[*i], "-n", 2) == 0)
		{
			j = 2;
			while (argv[*i][j] == 'n')
				j++;
			if (!argv[*i][j])
				*i += 1;
			else if (*i == 1 && argv[*i][j] != 'n')
				return (0);
			else
				break ;
		}
		return (1);
	}
	return (0);
}

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
	if (argv[i] != NULL && are_flags_n(argv, &i))
		line = 0;
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

