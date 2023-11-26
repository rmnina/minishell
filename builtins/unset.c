/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 10:19:45 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/22 18:06:44 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	unset_single_variable(char ***envp, const char *name)
{
	int	i;
	int	len;
	int	j;

	len = ft_strlen(name);
	i = 0;
	j = 0;
	while ((*envp)[i] != NULL)
	{
		if (ft_strncmp((*envp)[i], name, len) == 0 && (*envp)[i][len] == '=')
		{
			free((*envp)[i]);
			j = i;
			while ((*envp)[j] != NULL)
			{
				(*envp)[j] = (*envp)[j + 1];
				j++;
			}
			break ;
		}
		i++;
	}
}

int	my_unset(char ***envp, char **names)
{
	int	i;

	i = 0;
	while (names[i] != NULL)
	{
		unset_single_variable(envp, names[i]);
		i++;
	}
	return (0);
}

// int	main(int argc, char **argv)
// {
// 	(void)argv;
// 	char **fake_envp = malloc(3 * sizeof(char *));
// 	fake_envp[0] = strdup("PATH=/usr/bin");
// 	fake_envp[1] = strdup("HOME=/home/user");
// 	fake_envp[2] = NULL;
// 	if (argc > 1)
// 	{
// 		printf("\nVariables avant unset:\n");
// 		for (int i = 0; fake_envp[i] != NULL; i++) {
//             printf("%s\n", fake_envp[i]);
//         }
// 		my_unset(&fake_envp, argv + 1);
// 		printf("\nVariables après unset:\n");
// 		for (int i = 0; fake_envp[i] != NULL; i++) {
//             printf("%s\n", fake_envp[i]);
//         }
// 	}
// 	return (0);
// }
