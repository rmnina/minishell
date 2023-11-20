/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 02:11:36 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/20 15:58:37 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	error_quotes(t_quotes *quotes)
{
	if (quotes->single_quotes % 2 == 0)
	{
		if ((quotes->double_quotes % 2 != 0 && quotes->double_embedded % 2 == 0) \
		|| (quotes->double_quotes % 2 == 0 && quotes->double_embedded % 2 != 0))
			return (1);
	}
	if (quotes->double_quotes % 2 == 0)
	{
		if ((quotes->single_quotes % 2 != 0 && quotes->single_embedded % 2 == 0) \
		|| (quotes->single_quotes % 2 == 0 && quotes->single_embedded % 2 != 0))
			return (1);
	}
	return (0);
}
