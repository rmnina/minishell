/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 18:17:08 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/27 18:22:55 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char **parse_command_line(char *input)
{
    char *token;
    char *input_copy = strdup(input);  // Duplique la ligne de commande pour strtok
    int argc = 0;
    char **argv;
    
    if (!input_copy)
    {
        perror("strdup");
        return NULL;
    }
    token = strtok(input_copy, " ");
    while (token != NULL)
    {
        argc++;
        token = strtok(NULL, " ");
    }
    argv = malloc(sizeof(char*) * (argc + 1));  // +1 pour NULL à la fin
    if (!argv) {
        perror("malloc");
        free(input_copy);
        return NULL;
    }

    // Remplir argv avec les arguments
    strcpy(input_copy, input);  // Restaurer la ligne de commande
    int i = 0;
    token = strtok(input_copy, " ");
    while (token != NULL) {
        argv[i++] = strdup(token);
        token = strtok(NULL, " ");
    }
    argv[i] = NULL;  // Terminer argv avec NULL

    free(input_copy);
    return argv;
}

// Fonction pour libérer le tableau d'arguments
void free_parsed_command_line(char **argv) {
    if (argv != NULL) {
        for (int i = 0; argv[i] != NULL; i++) {
            free(argv[i]);
        }
        free(argv);
    }
}
