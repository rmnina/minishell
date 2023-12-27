/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 09:11:51 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/27 19:16:13 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	ft_export(char **envp, t_code *code)
// {
// 	int	i;

// 	i = 0;
// 	while (envp[i] != NULL)
// 	{
// 		printf("declare -x %s\n", envp[i]);
// 		i++;
// 	}
// 	code->code_status = 0;
// 	return (code->code_status);
// }

int	envp_length(char **envp)
{
	int	length;

	length = 0;
	while (envp[length] != NULL)
	{
		length++;
	}
	return (length);
}


char	**copy_envp(char **envp, int new_size, t_alloc *garbage)
{
	int		i;
	char	**new_envp;

	i = 0;
	new_envp = malloc(sizeof(char *) * (new_size + 1));
	if (new_envp == NULL)
	{
		//printf("Échec de l'allocation pour new_envp\n");
		return (NULL);
	}
	while (i < new_size)
	{
		if (envp[i] != NULL)
		{
			new_envp[i] = ft_strdup(envp[i], garbage);
			//printf("export %s\n", new_envp[i]);
			if (new_envp[i] == NULL)
			{
                perror("Failed to duplicate string");
                while (i > 0)
				{
                    free(new_envp[--i]);
                }
                free(new_envp);
                return NULL;
			}
		}
		else
		{
			new_envp[i] = NULL;
		}
		i++;
	}
	new_envp[new_size] = NULL;
	//printf("Copie de envp terminée avec succès.\n");
	return (new_envp);
}

void	add_or_update_env_var(char ***envp, char *var, t_alloc *garbage)
{
	printf("Ajout ou mise à jour de la variable d'environnement: %s\n", var);
	int		len;
	int		envp_len;
	char	**new_envp;
	int		i;
	bool found = false;

	i = 0;
	len = 0;
	envp_len = 0;
	new_envp = NULL;
	while (var[len] != '=' && var[len] != '\0')
		len++;
	envp_len = envp_length(*envp);
	new_envp = copy_envp(*envp, envp_len + 1, garbage);
	if (new_envp == NULL)
		return ;
	while (i < envp_len)
	{
		if (ft_strncmp(new_envp[i], var, len) == 0 && new_envp[i][len] == '=')
		{
			printf("Mise à jour de la variable '%s'\n", var);
			free(new_envp[i]);
			new_envp[i] = ft_strdup(var, garbage);
			if (new_envp[i] == NULL)
			{
                free(new_envp);
                return ;
            }
			found = true;
            break;
			//free(*envp);
			//*envp = new_envp;
			//return ;
		}
		i++;
	}
	printf("Ajout de la nouvelle variable '%s'\n", var);
	if (!found)
	{
		printf("Ajout de la nouvelle variable '%s'\n", var);
		new_envp[envp_len] = ft_strdup(var, garbage);
		new_envp[envp_len + 1] = NULL;
		printf("nouvelle variable: %s\n", var);
	}
	// if (new_envp[envp_len] == NULL)
	// {
    //     free(new_envp);
    //     return;
    // }
	// new_envp[envp_len + 1] = NULL;
	printf("Avant la mise à jour: *envp = %p\n", (void *)*envp);
	*envp = new_envp;
	printf("Après la mise à jour: *envp = %p\n", (void *)*envp);
}

int	ft_export(char ***envp, char **argv, t_code *code, t_alloc *garbage)
{
	//printf("Exécution de ft_export...\n");
	int	i;

	i = 0;
	if (argv[1] == NULL) 
	{
        while((*envp)[i] != NULL)
		{
			printf("export %s\n", (*envp)[i]);
			i++;
		}
		code->code_status = 0;
		return code->code_status;
	}
	else
	{
		i = 1;
		while (argv[i] != NULL)
		{
			printf("Avant modification, envp[%d] = %s\n", i, (*envp)[i]);
            printf("Ajout ou mise à jour de: %s\n", argv[i]);
			add_or_update_env_var(envp, argv[i], garbage);
			printf("Après modification, envp[%d] = %s\n", i, (*envp)[i]);
			i++;
		}
	}
	//printf("ft_export terminé avec succès.\n");
	code->code_status = 0;
	return (code->code_status);
}

