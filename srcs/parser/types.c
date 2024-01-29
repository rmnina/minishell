/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:04:44 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/29 17:35:02 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_in_quote(char c, t_parser *quotes)
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

int	dollar_type(t_minishell **main, int *i)
{
	if (!(*main)->line[*i + 1])
		return (0);
	else if ((*main)->line[*i + 1])
	{
		if ((*main)->line[*i + 1] == '?')
			return (CODE);
		else if (ft_isalnum((*main)->line[*i + 1]) \
		|| (*main)->line[*i + 1] == '_')
			return (EXPAND);
	}
	return (0);
}

int	special(t_minishell **main, int *i)
{
	if (!(*main)->line[*i])
		return (-1);
	if ((*main)->line[*i] == '|')
		return (PIPE);
	else if ((*main)->line[*i] == '<')
	{
		if ((*main)->line[*i + 1])
		{
			if ((*main)->line[*i + 1] == '<')
				return (DB_LEFT_CHEV);
		}
		return (LEFT_CHEV);
	}
	else if ((*main)->line[*i] == '>')
	{
		if ((*main)->line[*i + 1])
		{
			if ((*main)->line[*i + 1] == '>')
				return (DB_RIGHT_CHEV);
		}
		return (RIGHT_CHEV);
	}
	else if ((*main)->line[*i] == '$')
		return (dollar_type(main, i));
	return (0);
}

void	get_token_type(t_minishell **main, t_command *token)
{
	if (token->word[0] && token->word[0] == '|' \
	&& (*main)->parser->case_quotes == FALSE)
		token->type = PIPE;
	else if (token->word[0] && token->word[0] == '<' \
	&& token->word[1] && token->word[1] == '<' && \
	(*main)->parser->case_quotes == FALSE)
		token->type = DB_LEFT_CHEV;
	else if (token->word[0] && token->word[0] == '<' \
	&& (*main)->parser->case_quotes == FALSE)
		token->type = LEFT_CHEV;
	else if (token->word[0] && token->word[0] == '>' \
	&& token->word[1] && token->word[1] == '>' && \
	(*main)->parser->case_quotes == FALSE)
		token->type = DB_RIGHT_CHEV;
	else if (token->word[0] && token->word[0] == '>' \
	&& (*main)->parser->case_quotes == FALSE)
		token->type = RIGHT_CHEV;
	else if (token->word && is_expand(token->word) \
	&& (*main)->parser->case_single == FALSE)
		token->type = EXPAND;
	else
		token->type = WORD;
}

int	is_expand(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && ft_isalnum(line[i + 1]))
			return (1);
		i++;
	}
	return (0);
}
