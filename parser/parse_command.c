/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:17:08 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/06 12:12:52 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_word(t_command command)
{
	char	*word;
	int		size;
	int		i;

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

char	**parse_command_line(t_command *command)
{
	int		size;
	int		i;
	char	**arg;

	size = 0;
	i = 0;
	while (command[size].word)
		size++;
	arg = malloc(sizeof(char *) * size + 1);
	while (i < size - 1)
	{
		arg[i] = get_word(command[i]);
		i++;
	}
	arg[i] = NULL;
	return (arg);
}

char	**init_parsing(char *input)
{
	t_quotes	quotes;
	t_command	*command;
	t_expand	expand;
	char		**arg;
	int			size;

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



// char **parse_command_line(char *input)
// {
//     char *token;
//     char *input_copy = strdup(input);  // Duplique la ligne de commande pour strtok
//     int argc = 0;
//     char **argv;

//     if (!input_copy)
//     {
//         perror("strdup");
//         return NULL;
//     }
//     token = strtok(input_copy, " ");
//     while (token != NULL)
//     {
//         argc++;
//         token = strtok(NULL, " ");
//     }
//     argv = malloc(sizeof(char*) * (argc + 1));  // +1 pour NULL à la fin
//     if (!argv) {
//         perror("malloc");
//         free(input_copy);
//         return NULL;
//     }

//     // Remplir argv avec les arguments
//     strcpy(input_copy, input);  // Restaurer la ligne de commande
//     int i = 0;
//     token = strtok(input_copy, " ");
//     while (token != NULL) {
//         argv[i++] = strdup(token);
//         token = strtok(NULL, " ");
//     }
//     argv[i] = NULL;  // Terminer argv avec NULL

//     free(input_copy);
//     return argv;
// }

// // Fonction pour libérer le tableau d'arguments
// void free_parsed_command_line(char **argv) {
//     if (argv != NULL) {
//         for (int i = 0; argv[i] != NULL; i++) {
//             free(argv[i]);
//         }
//         free(argv);
//     }
// }