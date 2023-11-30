/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 00:19:58 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/30 15:07:47 by jdufour          ###   ########.fr       */
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
	else if (line[*i] == SPACE && quotes->case_double == FALSE && quotes->case_single == FALSE)
		return (1);
	return (0);
}

t_command	get_lex_expand(char *line, int *i, t_quotes *quotes)
{
	t_command		token;
	t_expand		expand;
	char			*var;
	static int		j = 0;

	var = init_get_expand(&token, line, i, &expand);
	*i += 1;
	while (ft_isalnum(line[*i]) || line[*i] == UNDERSCORE)
		*i += 1;
	while (var[j])
	{
		is_in_quote(var[j], quotes);
		special_types(var[j]);
		if (parse_quotes(var, &j, quotes) == 1)
			break;
		else if (!parse_quotes(var, &j, quotes))
		{
			token.word = ft_strjoin_char(token.word, var[j]);
			j++;
		}
	}
	token.type = WORD;
	if (var[j] != '\0')
		expand.left_expand = TRUE;
	return (token);
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
		else if (parse_quotes(line, i, quotes) == 1)
			break;
		else if (!is_in_quote(line[*i], quotes) && special_types(line[*i]) != 0 \
		&& special_types(line[*i]) != EXPAND)
		{
			if (token.word != NULL)
				break;
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
		if (token.word == NULL)
			i++;
		else
		{
			get_type(&token, quotes);
			command = ft_struct_join(command, token);
		}
	}
	command = ft_struct_join(command, token_null(&token));
	return (command);
}

// int	main(int argc, char **argv)
// {
// 	t_quotes	quotes;
// 	t_command	*command;
// 	t_expand	expand;
// 	// int			i = 0;

// 	expand.left_expand = FALSE;
// 	quotes.case_double = FALSE;
// 	quotes.case_single = FALSE;
// 	command = NULL;
// 	if (argc == 2)
// 	{
// 		error_quotes(argv[1], &quotes);
// 		command = get_command(argv[1], &quotes, &expand);
// 		ft_error_lexer(command);
// 		for(int i = 0; command[i].word != NULL; i++)
// 		{
// 			printf("word[%d] = %s\n", i, command[i].word);
// 			printf("type[%d] = %d\n", i, command[i].type);
// 		}
// 	}
// 	ft_free_command(command);
// }