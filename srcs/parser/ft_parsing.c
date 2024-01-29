/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parsing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 23:24:36 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/28 21:59:40 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	get_expand_token(t_minishell **main, int *i, t_alloc **garbage)
{
	t_command	token;

	init_get_token(&token);
	get_lex_expand(main, i, &token, garbage);
	return (token);
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
		if (!(*main)->line[i])
			break ;
		token = get_token(main, &i, garbage);
		if (!token.word && (*main)->parser->vpos != -1)
			token.word = garb_malloc(sizeof(char), 1, PARSING, garbage);
		if ((*main)->parser->vpos != -1 && \
        (!token.type || token.type < 1 || token.type > 8))
			token.type = WORD;
        if ((*main)->parser->vpos != -1)
		    command = ft_struct_join(command, token, PARSING, garbage);
		if ((*main)->parser->var != NULL)
		{
			while ((*main)->parser->var != NULL)
			{
				token = get_expand_token(main, &i, garbage);
				command = ft_struct_join(command, token, PARSING, garbage);
			}
		}
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