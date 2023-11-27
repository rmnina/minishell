/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 00:19:58 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/27 01:55:23 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	is_in_quote(char c, t_quotes *quotes)
{
	quotes->case_single = FALSE;
	quotes->case_double = FALSE;
	if (c == SINGLE_QUOTE && quotes->case_double == FALSE)
		quotes->case_single = TRUE;
	else if (c == DOUBLE_QUOTE && quotes->case_single == FALSE)
		quotes->case_double = TRUE;
	else if (c == SINGLE_QUOTE && quotes->case_single == TRUE)
		quotes->case_single = FALSE;
	else if (c == DOUBLE_QUOTE && quotes->case_double == TRUE)
		quotes->case_double = FALSE;
}

void	get_type(t_command *token)
{
	if (token->word[0] == '|')
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

t_command	get_token(char *line, t_quotes *quotes, int *i)
{
	t_command	token;
	
	token.word = NULL;
	while (line[*i])
	{
		is_in_quote(line[*i], quotes);
		if (line[*i] == SINGLE_QUOTE && quotes->case_double == FALSE)
			*i += 1;
		else if (line[*i] == DOUBLE_QUOTE && quotes->case_single == FALSE)
			*i += 1;
		else if ((line[*i] == SPACE && quotes->case_double == FALSE && quotes->case_single == FALSE))
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

t_command	*get_command(char *line, t_alloc *garbage, t_quotes *quotes)
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
		command = ft_struct_join(command, token);
	}
	add_to_garbage(command, garbage);
	return (command);
}

int	main(int argc, char **argv)
{
	t_quotes	quotes;
	t_alloc		*garbage;
	t_command	*command;
	// int			i = 0;

	if (argc == 2)
	{
		garbage = malloc(sizeof(garbage));
		garbage->adr = malloc(sizeof(void**));
		command = get_command(argv[1], garbage, &quotes);
		for(int i = 0; command[i].word != NULL; i++)
		{
			printf("word[%d] = %s\n", i, command[i].word);
			// printf("type[%d] = %d\n", i, command[i].type);
		}
	}
	ft_free_token(command);
	free(garbage->adr);
	free(garbage);
}