/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 20:37:10 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/29 16:45:44 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sigint_heredoc_handler(int signum)
{
	(void)signum;
	g_sigstatus = 130;
	exit(130);
}

void	sigint_heredoc_pipe_handler(int signum)
{
	int	fd_stdin;

	(void)signum;
	fd_stdin = open("/dev/tty", O_RDONLY);
	dup2(fd_stdin, STDIN_FILENO);
	close(fd_stdin);
	g_sigstatus = 130;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	init_heredoc_signal(void)
{
	signal(SIGINT, sigint_heredoc_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	init_heredoc_pipe_signal(void)
{
	signal(SIGINT, sigint_heredoc_pipe_handler);
	signal(SIGQUIT, SIG_IGN);
}
