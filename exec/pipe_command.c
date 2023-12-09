/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 14:59:17 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/09 23:30:52 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	split_command_for_pipes(char **cmd_args, t_command *command, \
t_pipe *pipes, int *i, t_alloc *garbage)
{
	pipes->command1 = cmd_args;
	*i += 1;
	pipes->command2 = create_cmd_args(command, i, garbage);
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

// int	commands_with_pipes_detected(char *input)
// {
// 	char	*pipe_position;

// 	pipe_position = ft_strchr(input, '|');

// 	if (pipe_position != NULL)
// 		return (1);
// 	return (0);
// }
