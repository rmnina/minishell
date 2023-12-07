/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:04:44 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/07 16:04:46 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

// Implementation of booleans whose changes of state
// indicates whether the character being processed is
// or isn't between quotes.

int	is_in_quote(char c, t_quotes *quotes)
{
	if (c == SINGLE_QUOTE && quotes->case_single == FALSE \
	&& quotes->case_double == FALSE)
		quotes->case_single = TRUE;
	else if (c == DOUBLE_QUOTE && quotes->case_double == FALSE \
	&& quotes->case_single == FALSE)
		quotes->case_double = TRUE;
	else if (c == SINGLE_QUOTE && quotes->case_single == TRUE)
		quotes->case_single = FALSE;
	else if (c == DOUBLE_QUOTE && quotes->case_double == TRUE)
		quotes->case_double = FALSE;
	if (quotes->case_single == TRUE || quotes->case_double == TRUE)
	{
		quotes->case_quotes = TRUE;
		return (1);
	}
	else
	{
		quotes->case_quotes = FALSE;
		return (0);
	}
}

// Just a silly hardcoded function to identify the type of
// an encountered special character. Types are defined in 
// an enum in the header file. This function is mostly used
// in the loop that verifies the input char by char, in order 
// to find out in which condition to enter.

int	special_types(char c)
{
	if (!c)
		return (-1);
	else if (c == '|')
		return (PIPE);
	else if (c == '<')
		return (LEFT_CHEV);
	else if (c == '>')
		return (RIGHT_CHEV);
	else if (c == '$')
		return (EXPAND);
	return (0);
}

// This function assigns a type to a token. Unlike the previous 
// function, which only performs a verification, this one writes 
// the type it has assign to the token into its structure t_command. 

void	get_type(t_command *token, t_quotes *quotes)
{
	if (!token->word)
		return ;
	else if (token->word[0] == '|' && quotes->case_quotes == FALSE)
		token->type = PIPE;
	else if (token->word[0] == '<' && token->word[1] && token->word[1] == '<' \
	&& quotes->case_quotes == FALSE)
		token->type = DB_LEFT_CHEV;
	else if (token->word[0] == '<' && quotes->case_quotes == FALSE)
		token->type = LEFT_CHEV;
	else if (token->word[0] == '>' && token->word[1] && token->word[1] == '>' \
	&& quotes->case_quotes == FALSE)
		token->type = DB_RIGHT_CHEV;
	else if (token->word[0] == '>' && quotes->case_quotes == FALSE)
		token->type = RIGHT_CHEV;
	else if (is_expand(token->word) && quotes->case_single == FALSE)
		token->type = EXPAND;
	else
		token->type = WORD;
}

// This function is just a loop to find if the input contains 
// at least one variable to expand. It's also only used in the
// loop that creates tokens.

int	is_expand(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == EXPAND)
			return (1);
		i++;
	}
	return (0);
}