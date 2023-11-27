/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joins.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 23:39:05 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/27 01:37:47 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_token(t_command *token)
{
	int	i;
	
	if (!token)
		return;
	i = 0;
	while (token[i].word != NULL)
	{
		if (token[i].word)
			free(token[i].word);
		i++;
	}
	free(token);
}

t_command	*ft_struct_join(t_command *tok1, t_command tok2)
{
	int			size;
	t_command	*res;
	int			i;

	i = 0;
	size = 0;
	if (tok1 != NULL)
	{
		while (tok1[size].word != NULL)
			size++;
	}
	res = malloc(sizeof(t_command) * (size + 2));
	if (!(res))
		return (NULL);
	while (i < size)
	{
		res[i] = tok1[i];
		i++;
	}
	res[size++] = tok2;
	res[size].word = NULL;
	if (tok1)
		free(tok1);
	return (res);
}
