/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 15:01:50 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/14 15:31:51 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int is_quotes(char *argvx, t_quotes *quotes)
{
	int i;

	i = 0;
	while (argvx[i])
	{
		if (argvx[i] == 39)
			quotes->single_quotes++;
		else if (argvx[i] == 34)
			quotes->double_quotes++;
		i++;
	}
}

