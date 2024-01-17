/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joins.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 23:39:05 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/17 19:19:12 by jdufour          ###   ########.fr       */
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

char	**ft_strjoin_args(t_minishell **main, int *i, t_alloc **garbage)
{
	char	**new_args;
	int		j;
	int		count;

	j = 0;
	count = 0;
	while ((*main)->cmd_args[count])
		count++;
	count++;
	new_args = garb_malloc(sizeof(char *), count + 1, garbage);
	while ((*main)->cmd_args[j])
	{
		new_args[j] = ft_strjoin(new_args[j], (*main)->cmd_args[j], garbage);
		j++;
	}
	new_args[j] = ft_strjoin(new_args[j], (*main)->command[*i].word, garbage);
	j++;
	new_args[j] = NULL;
	return (new_args);
}