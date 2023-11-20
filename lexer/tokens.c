/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 00:19:58 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/19 02:50:38 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	is_in_quote(char c, t_quotes *quotes)
{
	if (c == SINGLE_QUOTE && quotes->case_double == FALSE)
		quotes->case_single = TRUE;
	else if (c == DOUBLE_QUOTE && quotes->case_single == FALSE)
		quotes->case_double = TRUE;
	else if (c == SINGLE_QUOTE && quotes->case_single == TRUE)
		quotes->case_single = FALSE;
	else if (c == DOUBLE_QUOTE && quotes->case_double == TRUE)
		quotes->case_double = FALSE;
}

char	*get_token(char *line, char *token, t_quotes *quotes)
{
	int	i;
	
	i = 0;
	while (line[i])
	{
		is_in_quote(line[i], quotes);
		if (line[i] == SPACE && i == 0)
		{
			while (line[i] && line[i] == SPACE)
				i++;
		}
		else if (line[i] == SINGLE_QUOTE && quotes->case_double == FALSE)
			i++;
		else if (line[i] == DOUBLE_QUOTE && quotes->case_single == FALSE)
			i++;
		else if (line[i] == SPACE && quotes->case_double == FALSE && quotes->case_single == FALSE)
			break;
		else
		{
			ft_strjoin_char(token, line[i]);
			i++;
		}
	}
	return (token);
}

t_parsed	*allocate_token(char *line, t_quotes *quotes, t_alloc *garbage)
{
	t_parsed	*token;

	token = malloc(sizeof(t_parsed));
	if (!token)
	{
		free_garbage(garbage);
		return (NULL);
	}
	add_to_garbage(&token, garbage);
	token->word = malloc(sizeof(char));
	if (!token->word)
	{
		free_garbage(garbage);
		return (NULL);
	}
	add_to_garbage(&token->word, garbage);
	token->word[0] = '\0';
	get_token(line, token->word, quotes);
	printf("%s\n", token->word);
	return (token);
}

int	main(int argc, char **argv)
{
	t_quotes	quotes;
	t_alloc		*garbage;
	
	garbage = malloc(sizeof(garbage));
	garbage->adr = malloc(sizeof(void**));
	allocate_token(argv[1], &quotes, garbage);
}