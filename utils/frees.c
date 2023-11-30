/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 02:53:43 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/27 02:54:50 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_command(t_command *command)
{
	int	i;
	
	if (!command)
		return;
	i = 0;
	while (command[i].word != NULL)
	{
		if (command[i].word)
			free(command[i].word);
		i++;
	}
	free(command);
}