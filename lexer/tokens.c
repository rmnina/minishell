/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 00:19:58 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/18 22:52:18 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**get_token(char *line, t_quotes *quotes, t_alloc *garbage)
{
	char	**tokens;
	int		i;
	
	i = 0;
	if (quotes->is_quotes == TRUE)
	{
		if (error_quotes(quotes))
		{
			printf("syntax error near unexpected quote\n");
			exit(2);
		}
		else
		{
			
		}
	}
	else
		tokens = ft_split(line, ' ');
	add_to_garbage(&tokens, garbage);
	return (tokens);
}