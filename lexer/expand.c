/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:15:34 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/27 06:34:47 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	spaces(char *str, t_expand *pos)
{
	int	i;

	i = 0;
	pos->space = 0;
	while (str[i])
	{
		if (str[i] == SPACE)
			pos->space++;
		i++;
	}
}

t_expand	pos_expand(t_command *token)
{
	int			i;
	int			j;
	t_expand	pos;
	
	i = 0;
	while (token[i].word)
	{
		j = 0;
		while (token[i].word[j])
		{
			if (token[i].word[j] == EXPAND)
			{	
				pos.col = i;
				pos.row = i;
			}
			j++;
		}
		i++;
	}
	return (pos);
}

char	*get_name(t_command *token)
{
	t_expand		pos;
	char			*name;
	int				size;
	int				i;

	i = 0;
	pos = pos_expand(token);
	size = ft_strlen(token[pos.col].word);
	name = malloc(sizeof(char) * size + 1);
	if (!name)
		return (NULL);
	pos.row++;
	while (pos.row < size)
	{
		name[i] = token[pos.col].word[pos.row];
		i++;
		pos.row++;
	}
	name[i] = '\0';
	return (name);
}

// t_command   **tokens_concatenate(t_command **token)
// {
// 	int			i;
// 	char		*var;
// 	t_expand	pos;

// 	pos = pos_expand(*token);
// 	spaces(var, &pos);
// 	var = getenv(get_name(*token));
// 	i = 0;
// 	if (pos.row == 0)
// 	{
		
// 	}
	
	
// }
