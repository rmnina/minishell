/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:01:50 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/17 02:31:42 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quote(char c)
{
	if (c == SINGLE_QUOTE || c == DOUBLE_QUOTE)
		return (1);
	return (0);
}

void quotes_count(char *line, t_quotes *quotes)
{
	int i;

	i = 0;
	quotes->single_quotes = 0;
	quotes->double_quotes = 0;
	quotes->is_quotes = FALSE;
	while (line[i])
	{
		if (line[i] == SINGLE_QUOTE)
		{
			quotes->is_quotes = TRUE;
			quotes->single_quotes++;
		}
		else if (line[i] == DOUBLE_QUOTE)
		{
			quotes->is_quotes = TRUE;
			quotes->double_quotes++;
		}
		i++;
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
		{
			i++;
			while (line[i] && line[i] != SINGLE_QUOTE)
			{
				if (line[i] == DOUBLE_QUOTE)
					quotes->double_embedded++;
				i++;
			}
		}
		if (line[i] == DOUBLE_QUOTE)
		{
			i++;
			while (line[i] && line[i] != DOUBLE_QUOTE)
			{
				if (line[i] == SINGLE_QUOTE)
					quotes->single_embedded++;
				i++;
			}
		}
		i++;
	}
}

// int	error_quotes(t_quotes *quotes)
// {
// 	if (quotes->single_quotes % 2 == 0)
// 	{
// 		if ((quotes->double_quotes % 2 != 0 && quotes->double_embedded % 2 == 0) 
// 		|| (quotes->double_quotes % 2 == 0 && quotes->double_embedded % 2 != 0))
// 			return (1);
// 	}
// 	else if (quotes->double_quotes % 2 == 0)
// 	{
// 		if ((quotes->single_quotes % 2 != 0 && quotes->single_embedded % 2 == 0) 
// 		|| (quotes->single_quotes % 2 == 0 && quotes->single_embedded % 2 != 0))
// 			return (1);
// 	}
// 	return (0);
// }

// int	main(int argc, char **argv)
// {
// 	(void)argc;
// 	t_quotes quotes;
	
// 	quotes_count(argv[1], &quotes);
// 	embedded_quotes(argv[1], &quotes);
// 	printf("single embedded = %d\n", quotes.single_embedded);
// 	printf("double embedded = %d\n", quotes.double_embedded);
// 	printf("single = %d\n", quotes.single_quotes);
// 	printf("double = %d\n", quotes.double_quotes);
// 	if (error_quotes(&quotes))
// 		printf("error parsing\n");
// 	else
// 		printf("%s\n", argv[1]);
// 	return (0);
// }
