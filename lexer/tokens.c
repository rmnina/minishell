/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 00:19:58 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/07 11:11:35 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	get_special_type_token(char *line, int *i, t_quotes *quotes)
{
	t_command	token;

	token.word = NULL;
	if (special_types(line[*i]) == 3 && special_types(line[*i + 1]) == 3)
	{
		token.word = char_to_str(line[*i]);
		token.word = ft_strjoin_char(token.word, line[*i + 1]);
		*i += 2;
	}
	else if (special_types(line[*i]) == 4 && special_types(line[*i + 1]) == 4)
	{
		token.word = char_to_str(line[*i]);
		token.word = ft_strjoin_char(token.word, line[*i + 1]);
		*i += 2;
	}
	else
	{
		token.word = char_to_str(line[*i]);
		*i += 1;
	}
	get_type(&token, quotes);
	return (token);
}

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
		return (2);
	}
	else if (line[*i] == SPACE && quotes->case_double == FALSE \
	&& quotes->case_single == FALSE)
		return (1);
	return (0);
}

t_command	get_token(char *line, t_quotes *quotes, int *i, t_expand *expand)
{
	t_command	token;

	init_get_token(&token, expand);
	while (line[*i])
	{
		is_in_quote(line[*i], quotes);
		special_types(line[*i]);
		if (special_types(line[*i] == EXPAND) && quotes->case_single == FALSE \
		&& quotes->case_double == FALSE)
			token = get_lex_expand(line, i, quotes);
		else if (parse_quotes(line, i, quotes) == 1 || !line[*i])
			break ;
		else if (quotes->case_quotes == FALSE && special_types(line[*i]) != 0 \
		&& special_types(line[*i]) != EXPAND)
		{
			if (token.word != NULL)
				break ;
			else
				return (token = get_special_type_token(line, i, quotes));
		}
		else if (!parse_quotes(line, i, quotes))
		{
			token.word = ft_strjoin_char(token.word, line[*i]);
			*i += 1;
		}
	}
	return (token);
}

t_command	token_null(t_command *token)
{
	token->word = malloc(sizeof(char));
	token->word[0] = '\0';
	token->type = 0;
	return (*token);
}

t_command	*get_command(char *line, t_quotes *quotes, t_expand *expand)
{
	t_command	*command;
	t_command	token;
	int			i;

	i = 0;
	command = NULL;
	printf("get_command called with line: %s\n", line);
	while (line[i])
	{
		if (line[i] == SPACE)
		{
			while (line[i] && line[i] == SPACE)
				i++;
		}
		if (expand->left_expand == TRUE)
			token = get_lex_expand(line, &i, quotes);
		else
			token = get_token(line, quotes, &i, expand);
		printf("Token found: %s, Type: %d\n", token.word, token.type);
		if (token.word == NULL)
			i++;
		else
		{
			if (!token.type || token.type < 1 || token.type > 8)
				token.type = WORD;
			command = ft_struct_join(command, token);
		}
	}
	command = ft_struct_join(command, token_null(&token));
	printf("get_command finished processing\n");
	return (command);
}

// int	main(void)
// {
// 	t_quotes	quotes;
// 	t_command	*command;
// 	t_expand	expand;
// 	char		*line;

// 	expand.left_expand = FALSE;
// 	quotes.case_double = FALSE;
// 	quotes.case_single = FALSE;
// 	command = NULL;
// 	while (1)
// 	{
// 		line = readline("test_parsing > ");
// 		if (!line)
// 		{
// 			printf("exit ctrl+D\n");
// 			break ;
// 		}
// 		if (line[0] != 0)
// 		{
// 			add_history(line);
// 			error_quotes(line, &quotes);
// 			command = get_command(line, &quotes, &expand);
// 			ft_error_lexer(command);
// 			for (int i = 0; command[i + 1].word != NULL; i++)
// 			{
// 				printf("word[%d] = %s\n", i, command[i].word);
// 				printf("type[%d] = %d\n", i, command[i].type);
// 			}
// 		}
// 		clear_history();
// 		ft_free_command(command);
// 	}
// }