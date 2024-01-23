/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 20:04:47 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/23 20:05:06 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_parser	*get_parser(t_alloc **garbage)
{
	t_parser	*parser;

	parser = garb_malloc(sizeof(t_parser), 1, garbage);
	if (parser == NULL)
		return (NULL);
	return (parser);
}

t_parser	*init_parser(t_alloc **garbage)
{
	t_parser	*parser;

	parser = get_parser(garbage);
	if (parser == NULL)
		return (NULL);
	parser->case_double = FALSE;
	parser->case_single = FALSE;
	parser->case_quotes = FALSE;
	parser->var = NULL;
	parser->vpos = 0;
	return (parser);
}
