/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 23:24:36 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/29 17:35:14 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_token2(t_minishell **main, int *i, t_command *token, \
t_alloc **garbage)
{
	if ((*main)->parser->case_single == FALSE && \
	special(main, i) == CODE)
	{
		token->word = \
		ft_g_strjoin(token->word, ft_g_itoa \
		((*main)->code_status, PARSING, garbage), PARSING, garbage);
		*i += 2;
	}
	if (parse_quotes(main, i) == 1 || !(*main)->line[*i])
		return (-1);
	else if (!is_quotes(main, i) && !((*main)->parser->case_single == FALSE \
	&& (special(main, i) == EXPAND \
	|| special(main, i) == CODE)))
	{
		token->word = ft_strjoin_char(token->word, \
			(*main)->line[*i], PARSING, garbage);
		*i += 1;
	}
	return (0);
}

t_command	get_token1(t_minishell **main, int *i, t_alloc **garbage)
{
	t_command	token;

	init_get_token(&token);
	while ((*main)->line[*i])
	{
		is_in_quote((*main)->line[*i], (*main)->parser);
		if (special(main, i) == EXPAND && \
		(*main)->parser->case_single == FALSE)
		{
			get_lex_expand(main, i, &token, garbage);
			if (!(*main)->line[*i] || (*main)->line[*i] == 32)
				break ;
		}
		if ((*main)->parser->case_quotes == FALSE && special(main, i) != 0 \
		&& special(main, i) != EXPAND && special(main, i) != CODE)
		{
			if (token.word != NULL)
				break ;
			else
				return (token = get_special_type_token(main, i, garbage));
		}
		if (get_token2(main, i, &token, garbage) == -1)
			break ;
	}
	return (token);
}

t_command	*get_expand_token(t_minishell **main, t_command **command, \
int *i, t_alloc **garbage)
{
	t_command	token;

	while ((*main)->parser->var != NULL)
	{
		init_get_token(&token);
		get_lex_expand(main, i, &token, garbage);
		*command = ft_struct_join(*command, token, PARSING, garbage);
	}
	return (*command);
}

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
		(*main)->parser->vpos = 0;
		if (!(*main)->line[i])
			break ;
		token = get_token1(main, &i, garbage);
		if (!token.word && (*main)->parser->vpos != -1)
			token.word = garb_malloc(sizeof(char), 1, PARSING, garbage);
		if ((*main)->parser->vpos != -1 && \
		(!token.type || token.type < 1 || token.type > 8))
			token.type = WORD;
		if ((*main)->parser->vpos != -1)
			command = ft_struct_join(command, token, PARSING, garbage);
		if ((*main)->parser->var != NULL)
			command = get_expand_token(main, &command, &i, garbage);
	}
	return (command = ft_struct_join(command, \
	token_null(&token, garbage), PARSING, garbage));
}

t_command	*ft_parsing(t_minishell **main, t_alloc **garbage)
{
	t_command	*command;
	t_parser	*parser;

	command = NULL;
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
