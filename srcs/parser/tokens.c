/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:04:36 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/25 02:34:21 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	get_db_token(t_minishell **main, int *i, t_command *token, \
t_alloc **garbage)
{
	token->word = ft_strjoin_char(token->word, \
	(*main)->line[*i], ENV, garbage);
	*i += 1;
	if ((*main)->line[*i])
	{
		token->word = ft_strjoin_char(token->word, \
		(*main)->line[*i], ENV, garbage);
		*i += 1;
	}
}

t_command	get_special_type_token(t_minishell **main, int *i, \
t_alloc **garbage)
{
	t_command	token;

	token.word = NULL;
	if (special_types(main, i) == DB_LEFT_CHEV \
	|| special_types(main, i) == DB_RIGHT_CHEV \
	|| special_types(main, i) == CODE)
		get_db_token(main, i, &token, garbage);
	else
	{
		if ((*main)->line[*i])
		{
			token.word = ft_strjoin_char(token.word, \
			(*main)->line[*i], PARSING, garbage);
			*i += 1;
		}
	}
	get_token_type(main, &token);
	return (token);
}

int	parse_quotes(t_minishell **main, int *i)
{
	if (((*main)->parser->case_double == FALSE && \
	(*main)->parser->case_single == FALSE \
	&& (*main)->line[*i] == 32) || (*main)->line[*i] == '\0')
		return (1);
	if ((*main)->line[*i] == SINGLE_QUOTE && \
	(*main)->parser->case_double == FALSE)
	{
		*i += 1;
		return (2);
	}
	else if ((*main)->line[*i] == DOUBLE_QUOTE && \
	(*main)->parser->case_single == FALSE)
	{
		*i += 1;
		return (3);
	}
	return (0);
}

int	is_quotes(t_minishell **main, int *i)
{
	if (((*main)->parser->case_double == FALSE && \
	(*main)->parser->case_single == FALSE \
	&& (*main)->line[*i] == 32) || (*main)->line[*i] == '\0')
		return (1);
	if ((*main)->line[*i] == SINGLE_QUOTE && \
	(*main)->parser->case_double == FALSE)
		return (2);
	else if ((*main)->line[*i] == DOUBLE_QUOTE && \
	(*main)->parser->case_single == FALSE)
		return (3);
	return (0);
}

t_command	get_token(t_minishell **main, int *i, t_alloc **garbage)
{
	t_command	token;

	init_get_token(&token);
	(*main)->parser->vpos = 0;
	while ((*main)->line[*i])
	{
		is_in_quote((*main)->line[*i], (*main)->parser);
		if (special_types(main, i) == EXPAND && \
		(*main)->parser->case_single == FALSE)
		{
			get_lex_expand(main, i, &token, garbage);
			if (!(*main)->line[*i])
				break ;
		}
		if ((*main)->parser->case_quotes == FALSE && \
		special_types(main, i) != 0 \
		&& special_types(main, i) != EXPAND)
		{
			(*main)->parser->vpos = 0;
			if (token.word != NULL)
				break ;
			else
				return (token = get_special_type_token(main, i, garbage));
		}
		if (parse_quotes(main, i) == 1 || !(*main)->line[*i])
			break ;
		else if (!is_quotes(main, i) && special_types(main, i) != EXPAND)
		{
			token.word = ft_strjoin_char(token.word, \
			(*main)->line[*i], PARSING, garbage);
			*i += 1;
		}
	}
	return (token);
}
