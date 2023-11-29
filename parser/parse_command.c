/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:17:08 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/29 22:04:12 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char    *get_word(t_command command)
{
    char    *word;
    int     size;
    int     i;

    i = 0;
    size = 0;
    while (command.word[size])
        size++;
    word = malloc(sizeof(char) * size + 1);
    while (i < size)
    {
        word[i] = command.word[i];
        i++;
    }
    word[i] = '\0';
    return (word);
}

char    **parse_command_line(t_command *command)
{
    int size;
    int i;
    char    **arg;

    size = 0;
    i = 0;
    while (command[size].word)
        size++;
    arg = malloc(sizeof(char *) * size + 1);
    while(i < size - 1)
    {
        arg[i] = get_word(command[i]);
        i++;
    }
    arg[i] = NULL;
    return (arg);
}

char   **init_parsing(char *input)
{
    t_quotes	quotes;
	t_command	*command;
	t_expand	expand;
    char        **arg;
    int         size;

    expand.left_expand = FALSE;
	quotes.case_double = FALSE;
	quotes.case_single = FALSE;
	command = NULL;
    arg = NULL;
    error_quotes(input, &quotes);
	command = get_command(input, &quotes, &expand);
	ft_error_lexer(command);
    arg = parse_command_line(command);
    size = 0;
    while (arg[size])
        size++;
    return (arg);
}

void	free_parsed_command_line(char **res)
{
	int	i;

	i = 0;
	while (res[i])
		i++;
	while (i >= 0 && res[i])
	{
		free(res[i]);
		i--;
	}
	free (res);
}