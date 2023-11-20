/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/20 16:21:57 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>
#include <stdlib.h>

char *find_command_path(const char *command) 
{
    const char *path_env;
    char *path;
    char *full_path;
    char *start;
    char *end;
    
    if (ft_strchr(command, '/') != NULL)
        return (ft_strdup(command));
    path_env = getenv("PATH");
    if (!path_env)
        return (NULL);
    path = ft_strdup(path_env); // permet de ne pas modifier la variable d'env originale
    if (!path)
        return (NULL);
    full_path = malloc(PATH_MAX); //longueur max dans le systeme 
    if (!full_path)
    {
        free(path);
        return(NULL); 
    }
    start = path;
    while ((end = ft_strchr(start, ':')) != NULL) 
    {
        if (end != start) // si start n'est pas un ":"
        {
            ft_memcpy(full_path, start, end - start);
            full_path[end - start] = '\0';
            strcat(full_path, "/");
            strcat(full_path, command);

            if (access(full_path, X_OK) == 0) // fichier accessible et executable 
            {
                free(path);
                return full_path;
            }
        }
        start = end + 1;
    }
    if (*start) 
    {
        if(ft_strlen(start) + ft_strlen(command) + 2 <= PATH_MAX)
        {
            ft_strcpy(full_path, start);
            strcat(full_path, "/");
            strcat(full_path, command);
        }
        if (access(full_path, X_OK) == 0) 
        {
            free(path);
            return (full_path);
        }
    }
    free(path);
    free(full_path);
    return NULL;
}

// int main() 
// {
//     char *command = "ls";
//     char *command_path = find_command_path(command);

//     if (command_path)
//     {
//         printf("Chemin trouvé: %s\n", command_path);
//         free(command_path);
//     } 
//     else 
//     {
//         printf("Commande non trouvée.\n");
//     }

//     return 0;
// }


int main() 
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) 
    {
        char *command = "ls";
        char *path = find_command_path(command);
        if (!path)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        char *argv[] = { path, "-l", NULL };  // Remplacez par vos arguments
        char *envp[] = {NULL}; // peut etre obtenu par getenv
        
        execve(path, argv, envp);  // execve permet d'utiliser le PATH pour trouver ls
        perror("execve");  // execve ne retourne que s'il y a une erreur
        exit(EXIT_FAILURE);
    } 
    else 
    {
        waitpid(pid, &status, 0);  // Attendre que le processus enfant se termine
        if (WIFEXITED(status))
            printf("Processus enfant terminé avec le statut %d\n", WEXITSTATUS(status));
    }

    return (0);
}
