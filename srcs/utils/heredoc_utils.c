/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 06:32:53 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/14 00:09:34 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	heredoc_is_expand(char *line)
{
	int		i;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			if (line[i + 1] == '?')
				return (1);
			if (ft_isalnum(line[i + 1]) || line[i + 1] == UNDERSCORE)
				return (1);
		}
		i++;
	}
	return (0);
}

