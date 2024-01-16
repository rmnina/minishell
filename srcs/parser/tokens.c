/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:04:36 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/15 23:08:12 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// This functions processes built-ins, giving them a token all to
// themselves. This makes it possible to assign a type so that the 
// exec knows how to handle them. They are obviously considered
// literal if they're between quotes, but this verification will 
// be made by get_token().

t_command	get_special_type_token(t_minishell **main, int *i, t_alloc **garbage)
{
	t_command	token;

	token.word = NULL;
	if (special_types(main, i) == DB_LEFT_CHEV || special_types(main, i) == DB_RIGHT_CHEV \
	|| special_types(main, i) == CODE)
	{
		token.word = char_to_str((*main)->line[*i], garbage);
		token.word = ft_strjoin_char(token.word, (*main)->line[*i + 1], garbage);
		*i += 2;
	}
	else
	{
		token.word = char_to_str((*main)->line[*i], garbage);
		*i += 1;
	}
	get_token_type(main, &token);
	return (token);
}

// This was originally in the get_token() function, but had to be 
// separated so it could be used in other functions. It simply 
// tells the get_token() function how to proceed quotes, and returns
// one if it runs into a space that is not between quotes : this 
// indicates the end of the token.

int	parse_quotes(t_minishell **main, int *i)
{
	if (((*main)->parser->case_double == FALSE && (*main)->parser->case_single == FALSE \
	&& (*main)->line[*i] == 32) || (*main)->line[*i] == '\0')
		return (1);
	if ((*main)->line[*i] == SINGLE_QUOTE && (*main)->parser->case_double == FALSE)
	{
		*i += 1;
		return (2);
	}
	else if ((*main)->line[*i] == DOUBLE_QUOTE && (*main)->parser->case_single == FALSE)
	{
		*i += 1;
		return (3);
	}
	return (0);
}

int	is_quotes(t_minishell **main, int *i)
{
	if (((*main)->parser->case_double == FALSE && (*main)->parser->case_single == FALSE \
	&& (*main)->line[*i] == 32) || (*main)->line[*i] == '\0')
		return (1);
	if ((*main)->line[*i] == SINGLE_QUOTE && (*main)->parser->case_double == FALSE)
		return (2);
	else if ((*main)->line[*i] == DOUBLE_QUOTE && (*main)->parser->case_single == FALSE)
		return (3);
	return (0);
}

// this function is one of the most importants as it creates the tokens,
// which are the structures t_command that are gonna be placed in the array
// treated by the exec. 
t_command	get_token(t_minishell **main, int *i, t_alloc **garbage)
{
	t_command	token;

	init_get_token(&token);
	while ((*main)->line[*i])
	{
		is_in_quote((*main)->line[*i], (*main)->parser);
		if (special_types(main, i) == EXPAND && (*main)->parser->case_single == FALSE)
			get_lex_expand(main, i, &token, garbage);
		if ((*main)->parser->case_quotes == FALSE && special_types(main, i) != 0 \
		&& special_types(main, i) != EXPAND)
		{
			if (token.word != NULL)
				break ;
			else
				return (token = get_special_type_token(main, i, garbage));
		}
		if (parse_quotes(main, i) == 1 || !(*main)->line[*i])
			break ;
		else if (!is_quotes(main, i) && special_types(main, i) != EXPAND)
		{
			// printf("line = %c\n", (*main)->line[*i]);
			token.word = ft_strjoin_char(token.word, (*main)->line[*i], garbage);
			*i += 1;
		}
	}
	return (token);
}

// This functions creates the array with all of the structures. It also
// verifies if the env variable have been treated accordingly, and gets 
// the rest of them if not.

t_command	*get_command(t_minishell **main, t_alloc **garbage)
{
	t_command	*command;
	t_command	token;
	int			i;

	i = 0;
	command = NULL;
	while ((*main)->line[i])
	{
		check_spaces(main, &i);
		if (!(*main)->line[i])
			break ;
		token = get_token(main, &i, garbage);
		if (!token.word)
			token.word = garb_malloc(sizeof(char), 1, garbage);
		if (!token.type || token.type < 1 || token.type > 8)
			token.type = WORD;
		command = ft_struct_join(command, token, garbage);
		if ((*main)->parser->var != NULL)
		{
			while ((*main)->parser->var != NULL)
			{
				init_get_token(&token);
				get_lex_expand(main, &i, &token, garbage);
				command = ft_struct_join(command, token, garbage);
			}
		}
	}
	return (command = ft_struct_join(command, token_null(&token, garbage), garbage));
}

t_command	*ft_parsing(t_minishell **main, t_alloc **garbage)
{
	t_command	*command;
	t_parser	*parser;

	parser = init_parser(garbage);
	if (parser == NULL)
		return (NULL);
	(*main)->parser = parser;
	if (error_quotes(main) == -1)
		return (command = NULL);
	command = get_command(main, garbage);
	if (ft_error_lexer(command, main) == -1)
		return (command = NULL);
	return (command);
}
