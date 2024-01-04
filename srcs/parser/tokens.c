/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:04:36 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/04 17:39:31 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// This functions processes built-ins, giving them a token all to
// themselves. This makes it possible to assign a type so that the 
// exec knows how to handle them. They are obviously considered
// literal if they're between quotes, but this verification will 
// be made by get_token().

t_command	get_special_type_token(char *line, int *i, t_quotes *quotes, t_alloc *garbage)
{
	t_command	token;

	token.word = NULL;
	if (special_types(line[*i], line[*i + 1]) == DB_LEFT_CHEV \
	|| special_types(line[*i], line[*i + 1]) == DB_RIGHT_CHEV \
	|| special_types(line[*i], line[*i + 1]) == CODE)
	{
		token.word = char_to_str(line[*i], garbage);
		token.word = ft_strjoin_char(token.word, line[*i + 1], garbage);
		*i += 2;
	}
	else
	{
		token.word = char_to_str(line[*i], garbage);
		*i += 1;
	}
	get_type(&token, quotes);
	return (token);
}

// This was originally in the get_token() function, but had to be 
// separated so it could be used in other functions. It simply 
// tells the get_token() function how to proceed quotes, and returns
// one if it runs into a space that is not between quotes : this 
// indicates the end of the token.

int	parse_quotes(char *line, int *i, t_quotes *quotes)
{
	if (line[*i] == SINGLE_QUOTE && quotes->case_double == FALSE)
	{
		*i += 1;
		return (2);
	}
	else if (line[*i] == DOUBLE_QUOTE && quotes->case_single == FALSE)
	{
		*i += 1;
		return (3);
	}
	else if ((line[*i] == SPACE && quotes->case_double == FALSE \
	&& quotes->case_single == FALSE) || line[*i] == '\0')
		return (1);
	return (0);
}

// this function is one of the most importants as it creates the tokens,
// which are the structures t_command that are gonna be placed in the array
// treated by the exec. 

t_command	get_token(char *line, t_quotes *quotes, int *i, t_alloc *garbage, char ***envp)
{
	t_command	token;

	init_get_token(&token);
	while (line[*i])
	{
		is_in_quote(line[*i], quotes);
		if (parse_quotes(line, i, quotes) == 1 || !line[*i])
			break ;
		if ((special_types(line[*i], line[*i + 1]) == EXPAND \
		&& quotes->case_single == FALSE) || quotes->var != NULL)
		{
			if (get_lex_expand(line, i, quotes, &token, garbage, envp) == 1)
				break ;
		}
		if (quotes->case_quotes == FALSE && special_types(line[*i], line[*i + 1]) != 0 \
		&& special_types(line[*i], line[*i + 1]) != EXPAND)
		{
			if (token.word != NULL)
				break ;
			else
				return (token = get_special_type_token(line, i, quotes, garbage));
		}
		else if (!parse_quotes(line, i, quotes))
		{
			token.word = ft_strjoin_char(token.word, line[*i], garbage);
			*i += 1;
		}
	}
	return (token);
}

// This function creates a null t_command token. It will be added at the
// end of the array, so we can iterate on it by having an exit condition.

t_command	token_null(t_command *token, t_alloc *garbage)
{
	token->word = garb_malloc(sizeof(char), 1, &garbage);
	token->word[0] = '\0';
	token->type = 0;
	return (*token);
}

// This functions creates the array with all of the structures. It also
// verifies if the env variable have been treated accordingly, and gets 
// the rest of them if not.

t_command	*get_command(char *line, t_quotes *quotes, t_alloc *garbage, char ***envp)
{
	t_command	*command;
	t_command	token;
	int			i;

	i = 0;
	command = NULL;
	while (line[i])
	{
		if (line[i] == SPACE)
		{
			while (line[i] && line[i] == SPACE)
				i++;
		}
		token = get_token(line, quotes, &i, garbage, envp);
		if (token.word == NULL)
			i++;
		else
		{
			if (!token.type || token.type < 1 || token.type > 8)
				token.type = WORD;
			command = ft_struct_join(command, token, garbage);
		}
	}
	if (!line[i] && quotes->var != NULL)
	{
		while (quotes->var != NULL)
		{
			get_lex_expand(line, &i, quotes, &token, garbage, envp);
			command = ft_struct_join(command, token, garbage);
		}
	}
	command = ft_struct_join(command, token_null(&token, garbage), garbage);
	// for (int j = 0; command[j].type != 0; j++)
	// 	printf("token = %s\n", command[j].word);
	return (command);
}
