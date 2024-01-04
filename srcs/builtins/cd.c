/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 17:31:57 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/04 17:30:44 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_cd(char **args, t_code *code)
{
	const char	*path;

	if (args[1] == NULL)
	{
		path = getenv("HOME");
		if (path == NULL)
		{
			code->code_status = 1;
			return (code->code_status);
		}
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		code->code_status = 1;
		return (code->code_status);
	}
	return (code->code_status);
}



// int	ft_cd(char **args, char ***envp, t_code *code, t_alloc *garbage)
// {
// 	char	old_path[PATH_MAX];
// 	char	new_path[PATH_MAX];
// 	char	*path;

// 	path = NULL;
// 	//printf("PWD actuel: %s, OLDPWD actuel: %s\n", getenv("PWD"), getenv("OLDPWD"));
// 	if (!getcwd(old_path, sizeof(old_path)))
// 	{
// 		perror("cd: error getting current directory");
// 		code->code_status = 1;
// 		return (code->code_status);
// 	}
// 	//printf("Répertoire courant avant cd: %s\n", old_path);
// 	if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
// 		path = getenv("HOME");
// 	else if (args[1] && ft_strcmp(args[1], "-") == 0)
// 	{
// 		path = getenv("OLDPWD");
// 		//printf("Chemin OLDPWD: %s\n", path);
// 		if (path == NULL)
// 		{
// 			printf("cd: OLDPWD not set\n");
// 			code->code_status = 1;
// 			return (code->code_status);
// 		}
// 		//printf("Changement de répertoire vers OLDPWD: %s\n", path); 
// 	}
// 	else
// 	{
// 		path = args[1];
// 	}
// 	if (chdir(path) != 0)
// 	{
// 		perror("cd");
// 		code->code_status = 1;
// 		return (code->code_status);
// 	}
// 	if (!getcwd(new_path, sizeof(new_path)))
// 	{
// 		perror("cd: error getting new directory");
// 		code->code_status = 1;
// 		return (code->code_status);
// 	}
// 	//printf("Variable 'OLDPWD' avant mise à jour '%s'\n", old_path);
// 	add_or_update_env_var(envp, "OLDPWD", garbage);
// 	//printf("Variable 'OLDPWD' apres mise à jour '%s'\n", old_path);
// 	//printf("Variable PWD avant mise a jour: %s\n", new_path);
// 	add_or_update_env_var(envp, "PWD", garbage);
// 	//printf("Variable 'PWD' apres mise à jour '%s'\n", new_path);
// 	return (code->code_status);
// }



