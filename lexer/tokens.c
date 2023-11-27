/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 00:19:58 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/27 06:36:00 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	get_special_type_token(char *line, int *i)
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
	get_type(&token);
	return (token);
}

t_command	get_token(char *line, t_quotes *quotes, int *i)
{
	t_command	token;
	
	token.word = NULL;
	quotes->case_single = FALSE;
	quotes->case_double = FALSE;
	while (line[*i])
	{
		is_in_quote(line[*i], quotes);
		special_types(line[*i]);
		if (line[*i] == SINGLE_QUOTE && quotes->case_double == FALSE)
			*i += 1;
		else if (line[*i] == DOUBLE_QUOTE && quotes->case_single == FALSE)
			*i += 1;
		else if (!is_in_quote(line[*i], quotes) && special_types(line[*i]) != 0 \
		&& special_types(line[*i]) != 7)
		{
			if (token.word != NULL)
				break;
			else 
				return (token = get_special_type_token(line, i));
		}
		else if ((line[*i] == SPACE && quotes->case_double == FALSE \
		&& quotes->case_single == FALSE))
			break;
		else
		{
			token.word = ft_strjoin_char(token.word, line[*i]);
			*i += 1;
		}
	}
	get_type(&token);
	return (token);
}

t_command	*get_command(char *line, t_quotes *quotes)
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
		token = get_token(line, quotes, &i);
		if (token.word == NULL)
			i++;
		else
			command = ft_struct_join(command, token);
	}
	return (command);
}

// int	main(int argc, char **argv)
// {
// 	t_quotes	quotes;
// 	t_command	*command;
// 	// int			i = 0;

// 	quotes.case_double = FALSE;
// 	quotes.case_single = FALSE;
// 	command = NULL;
// 	if (argc == 2)
// 	{
// 		error_quotes(argv[1], &quotes);
// 		command = get_command(argv[1], &quotes);
// 		ft_error_lexer(command);
// 		for(int i = 0; command[i].word != NULL; i++)
// 		{
// 			printf("word[%d] = %s\n", i, command[i].word);
// 			printf("type[%d] = %d\n", i, command[i].type);
// 		}
// 	}
// 	ft_free_command(command);
// }