/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joins.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/26 23:39:05 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/27 04:28:15 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_command	*ft_struct_join(t_command *tok1, t_command tok2)
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


char	*char_to_str(char c)
{
	char	*res;

	res = malloc(sizeof(char) * 2);
	res[1] = '\0';
	res[0] = c;
	return (res); 
}