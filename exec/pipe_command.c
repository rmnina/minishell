/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 14:59:17 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/06 18:01:24 by juandrie         ###   ########.fr       */
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
		printf("Commande 1: '%s', Commande 2: '%s'\n", pipes->command1, pipes->command2);
	}
}

// void	split_command_for_pipes(t_command *command, t_pipe *pipes)
// {
// 	char	*pipe_ptr;

// 	pipe_ptr = ft_strchr(command->file, '|');
// 	if (pipe_ptr)
// 	{
// 		*pipe_ptr = '\0';
// 		pipes->command1 = command->word;
// 		pipes->command2 = pipe_ptr + 1;
// 	}
// }

int	commands_with_pipes_detected(char *input)
{
	char	*pipe_position;

	pipe_position = ft_strchr(input, '|');

	if (pipe_position != NULL)
		return (1);
	return (0);
}
