/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 09:11:51 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/04 17:17:53 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_export(char **envp, t_code *code)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		printf("declare -x %s\n", envp[i]);
		i++;
	}
	code->code_status = 0;
	return (code->code_status);
}

// int envp_length(char **envp) {
//     int length = 0;
//     while (envp[length] != NULL) {
//         length++;
//     }
//     return length;
// }

// void free_envp(char **envp) {
//     int i = 0;
//     while (envp[i] != NULL) {
//         free(envp[i]);
//         i++;
//     }
//     free(envp);
// }

// char **copy_envp(char **envp, int new_size) {
//     char **new_envp = malloc(sizeof(char *) * (new_size + 1));
//     if (new_envp == NULL) {
//         return NULL; // Gérer l'erreur d'allocation
//     }
//     for (int i = 0; i < new_size; i++) {
//         if (envp[i] != NULL) {
//             new_envp[i] = strdup(envp[i]);
//         } else {
//             new_envp[i] = NULL;
//         }
//     }
//     new_envp[new_size] = NULL;
//     return new_envp;
// }

// void add_or_update_env_var(char ***envp, const char *var) {
//     int len = 0;
//     while (var[len] != '=' && var[len] != '\0') len++;

//     int envp_len = envp_length(*envp);
//     char **new_envp = copy_envp(*envp, envp_len + 1); // +1 pour un éventuel nouvel élément
//     if (new_envp == NULL) return;

//     for (int i = 0; i < envp_len; i++) {
//         if (strncmp(new_envp[i], var, len) == 0 && new_envp[i][len] == '=') {
//             free(new_envp[i]);
//             new_envp[i] = strdup(var);
//             free_envp(*envp);
//             *envp = new_envp;
//             return;
//         }
//     }

//     new_envp[envp_len] = strdup(var);
//     new_envp[envp_len + 1] = NULL;
//     free_envp(*envp);
//     *envp = new_envp;
// }

// int ft_export(char ***envp, char **argv, t_code *code)
// {
//     int	i;

// 	i = 0;
// 	while (argv[i] != NULL)
// 	{
// 		add_or_update_env_var(envp, argv[i]);
// 		i++;
// 	}
// 	code->code_status = 0;
// 	return (code->code_status);
// }

