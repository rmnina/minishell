/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joins.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 23:39:05 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/10 00:41:11 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*ft_struct_join(t_command *tok1, t_command tok2, t_alloc *garbage)
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
	res = garb_malloc(sizeof(t_command), (size + 2), &garbage);
	if (!(res))
		return (NULL);
	while (i < size)
	{
		res[i] = tok1[i];
		i++;
	}
	res[size++] = tok2;
	res[size].word = NULL;
	// if (tok1)
	// 	free(tok1);
	return (res);
}


char	*char_to_str(char c, t_alloc *garbage)
{
	char	*res;

	res = garb_malloc(sizeof(char), 2, &garbage);
	res[1] = '\0';
	res[0] = c;
	return (res); 
}