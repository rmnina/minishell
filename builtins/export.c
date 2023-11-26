/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 09:11:51 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/22 17:34:33 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	my_export(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		printf("declare -x %s\n", envp[i]);
		i++;
	}
	return (0);
}

// int	main(int ac, char **av, char **envp)
// {
// 	if (ac > 1)
// 		export(av[1]);
// 	return (0);
// }
