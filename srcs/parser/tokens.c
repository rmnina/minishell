/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 16:04:36 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/29 17:35:02 by jdufour          ###   ########.fr       */
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
	if (special(main, i) == DB_LEFT_CHEV \
	|| special(main, i) == DB_RIGHT_CHEV)
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
