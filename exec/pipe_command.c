/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 14:59:17 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/12 18:28:47 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	split_command_for_pipes(char *input, t_pipe *pipes)
// {
// 	char	*pipe_ptr;

// 	pipe_ptr = ft_strchr(input, '|');
// 	if (pipe_ptr)
// 	{
// 		*pipe_ptr = '\0';
// 		pipes->command1 = ft_strtrim(input, " ");
// 		pipes->command2 = ft_strtrim(pipe_ptr + 1, " ");
// 	}
// }

void	split_command_for_pipes(char **cmd_args, t_command *command, \
t_pipe *pipes, int *i, t_alloc *garbage)
{
	pipes->command1 = cmd_args;
	*i += 1;
	pipes->command2 = create_cmd_args(command, i, garbage);
}
