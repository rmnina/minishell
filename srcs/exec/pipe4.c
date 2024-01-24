/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:56:52 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 18:57:22 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	pipex_loop(t_minishell **main, int *i, t_alloc **garbage)
{
	if (is_heredoc(main, i))
		(*main)->heredoc = ft_heredoc(main, i, garbage);
	if (is_first_pipe(main, i))
		first_pipe(main, i, garbage);
	else
		middle_pipe(main, i, garbage);
	if (check_redir(main, i) == 0)
	{
		while ((*main)->command[*i].type && (*main)->command[*i].type != PIPE)
			(*i)++;
	}
	(*i)++;
	(*main)->cmd_args = create_cmd_args(main, i, garbage);
	(*main)->heredoc = 0;
}

int	ft_pipex(t_minishell **main, int *i, t_alloc **garbage)
{
	int	fd_stdin;

	fd_stdin = open("/dev/tty", O_RDONLY);
	init_process_signal();
	while ((*main)->nb_cmd < (*main)->total_cmd - 1)
		pipex_loop(main, i, garbage);
	if (is_heredoc(main, i))
		(*main)->heredoc = ft_heredoc(main, i, garbage);
	last_pipe(main, i, garbage);
	dup2(fd_stdin, STDIN_FILENO);
	close(fd_stdin);
	wait_pids(main);
	restore_fds(main);
	return (0);
}
