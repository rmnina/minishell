/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 04:33:44 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/27 04:57:59 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_in_quote(char c, t_quotes *quotes)
{
	if (c == SINGLE_QUOTE && quotes->case_single == FALSE && quotes->case_double == FALSE)
		quotes->case_single = TRUE;
	else if (c == DOUBLE_QUOTE && quotes->case_double == FALSE && quotes->case_single == FALSE)
		quotes->case_double = TRUE;
	else if (c == SINGLE_QUOTE && quotes->case_single == TRUE)
		quotes->case_single = FALSE;
	else if (c == DOUBLE_QUOTE && quotes->case_double == TRUE)
		quotes->case_double = FALSE;
	if (quotes->case_single == TRUE || quotes->case_double == TRUE)
		return (1);
	else
		return (0);
}

int	special_types(char c)
{
	if (!c)
		return (0);
	else if (c == '|')
		return(PIPE);
	else if (c == '<')
		return(LEFT_CHEV);
	else if (c == '>')
		return(RIGHT_CHEV);
	else if (c == '$')
		return(EXPAND);
	return(0);
}

void	get_type(t_command *token)
{
	if (!token->word)
		return;
	else if (token->word[0] == '|')
		token->type = PIPE;
	else if (token->word[0] == '<' && token->word[1] && token->word[1] == '<')
		token->type = DB_LEFT_CHEV;
	else if (token->word[0] == '<')
		token->type = LEFT_CHEV;
	else if (token->word[0] == '>' && token->word[1] && token->word[1] == '>')
		token->type = DB_RIGHT_CHEV;
	else if (token->word[0] == '>')
		token->type = RIGHT_CHEV;
	else if (token->word[0] == '$')
		token->type = EXPAND;
	else
		token->type = WORD;
}