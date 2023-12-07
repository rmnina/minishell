/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 14:59:17 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/07 17:40:28 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	split_command_for_pipes(char *input, t_pipe *pipes)
{
	char	*pipe_ptr;

	pipe_ptr = ft_strchr(input, '|');
	if (pipe_ptr)
	{
		*pipe_ptr = '\0';
		pipes->command1 = ft_strtrim(input, " ");
		pipes->command2 = ft_strtrim(pipe_ptr + 1, " ");
	}
}

int	commands_with_pipes_detected(char *input)
{
	char	*pipe_position;

	pipe_position = ft_strchr(input, '|');

	if (pipe_position != NULL)
		return (1);
	return (0);
}
