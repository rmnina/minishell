/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:01:50 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/20 15:54:32 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void quotes_count(char *line, t_quotes *quotes)
{
	int i;

	i = 0;
	quotes->single_quotes = 0;
	quotes->double_quotes = 0;
	while (line[i])
	{
		if (line[i] == SINGLE_QUOTE)
			quotes->single_quotes++;
		else if (line[i] == DOUBLE_QUOTE)
			quotes->double_quotes++;
		i++;
	}
}

void	double_embedded(char *line, t_quotes *quotes, int *i)
{
	*i++;
	while (line[*i] && line[*i] != SINGLE_QUOTE)
	{
		if (line[*i] == DOUBLE_QUOTE)
			quotes->double_embedded++;
		*i++;
	}
}

void	single_embedded(char *line, t_quotes *quotes, int *i)
{
	*i++;
	while (line[*i] && line[*i] != DOUBLE_QUOTE)
	{
		if (line[*i] == SINGLE_QUOTE)
			quotes->single_embedded++;
		*i++;
	}
}

void	embedded_quotes(char *line, t_quotes *quotes)
{
	int	i;

	i = 0;
	quotes->single_embedded = 0;
	quotes->double_embedded = 0;
	while (line[i])
	{
		if (line[i] == SINGLE_QUOTE)
			double_embedded(line, quotes, &i);
		else if (line[i] == DOUBLE_QUOTE)
			single_embedded(line, quotes, &i);
		i++;
	}
}

int	main(int argc, char **argv)
{
	(void)argc;
	t_quotes quotes;
	
	quotes_count(argv[1], &quotes);
	embedded_quotes(argv[1], &quotes);
	printf("single embedded = %d\n", quotes.single_embedded);
	printf("double embedded = %d\n", quotes.double_embedded);
	printf("single = %d\n", quotes.single_quotes);
	printf("double = %d\n", quotes.double_quotes);
	if (error_quotes(&quotes))
		printf("error parsing\n");
	else
		printf("%s\n", argv[1]);
	return (0);
}
