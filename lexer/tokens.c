/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 00:19:58 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/17 02:15:33 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**get_token(char *line, t_quotes *quotes, t_alloc *garbage)
{
	char	**tokens;
	int		i;
	
	i = 0;
	// if (quotes->is_quotes == TRUE)
	// {
	// 	while (line[i])
			
	// }
	// else
		tokens = ft_split(line, ' ');
	add_to_garbage(&tokens, garbage);
	return (tokens);
}