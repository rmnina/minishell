/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joins.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 23:39:05 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/18 13:49:25 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*ft_struct_join(t_command *tok1, t_command tok2, t_alloc **garbage)
{
	int			size;
	t_command	*res;
	int			i;

	i = 0;
	size = 0;
	res = NULL;
	if (tok1 != NULL)
	{
		while (tok1[size].word != NULL)
			size++;
	}
	res = garb_malloc(sizeof(t_command), (size + 2), garbage);
	if (!(res))
		return (NULL);
	while (i < size)
	{
		res[i] = tok1[i];
		i++;
	}
	res[size++] = tok2;
	res[size].word = NULL;
	return (res);
}

char	*char_to_str(char c, t_alloc **garbage)
{
	char	*res;

	res = garb_malloc(sizeof(char), 2, garbage);
	res[1] = '\0';
	res[0] = c;
	return (res);
}

char	**ft_envjoin(char **envp, char *str, t_alloc **garbage)
{
	char	**new_envp;
	int		size;
	int		index;

	size = 0;
	index = 0;
	while (envp[size])
		size++;
	new_envp = garb_malloc(sizeof(char *), (size + 2), garbage);
	while (envp[index])
	{
		new_envp[index] = garb_malloc(sizeof(char), (ft_strlen(envp[index]) + 1), garbage);
		ft_strcpy(new_envp[index], envp[index]);
		index++;
	}
	new_envp[size] = garb_malloc(sizeof(char), (ft_strlen(str) + 1), garbage);
	new_envp[size + 1] = NULL;
	ft_strcpy(new_envp[size], str);
	return (new_envp);
}

