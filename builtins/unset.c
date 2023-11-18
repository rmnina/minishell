/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 10:19:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/14 11:21:28 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


EnvVar *add_env_var(const char *name, const char *value) 
{
    EnvVar *new_var = malloc(sizeof(EnvVar));
    if (!new_var) 
        return NULL;
    new_var->name = strdup(name);
    new_var->value = strdup(value);
    new_var->next = env_vars;
    env_vars = new_var;

    return new_var;
}

void print_env_vars() 
{
    EnvVar *current = env_vars;
    
    while ( current != NULL) 
    {
        printf("%s=%s\n", current->name, current->value);
        current = current->next;
    }
}

void unset(const char *name) 
{
    EnvVar *current = env_vars;
    EnvVar *previous = NULL;
    
    while (current != NULL) 
    {
        if (strcmp(current->name, name) == 0) 
        {
            if(previous == NULL)
                env_vars = current->next;
            else
                previous->next = current->next;
            free(current->name);
            free(current->value);
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}


void free_env_vars() 
{
    EnvVar *current = env_vars;
    while (current != NULL) 
    {
        EnvVar *next = current->next;
        free(current->name);
        free(current->value);
        free(current);
        current = next;
    }
}

int main(int ac, char **av) 
{
    
    if (ac > 1)
    {
        // Ajouter quelques variables d'environnement
        add_env_var("PATH", "/usr/bin");
        add_env_var("HOME", "/home/user");

        // Afficher les variables d'environnement
        printf("Variables avant unset:\n");
        print_env_vars(av[1]);

        // Supprimer une variable
        unset("PATH");

        // Afficher à nouveau
        printf("\nVariables après unset:\n");
        print_env_vars(av[1]);

        // Libérer les ressources avant de quitter
        free_env_vars(av[1]);
    }
    return 0;
}
