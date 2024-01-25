/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julietteandrieux <julietteandrieux@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:13:20 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/25 20:28:22 by julietteand      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sigint_handler(int signum)
{	
	if (signum == SIGINT)
	{
		g_sigstatus = 130;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sigint_process_handler(int signum)
{
	(void)signum;
	g_sigstatus = 130;
	printf("\n");
}

void	sigquit_process_handler(int signum)
{
	(void)signum;
	g_sigstatus = 131;
	printf("Quit (core dumped)\n");
}

void	init_signal(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	init_process_signal(void)
{
	signal(SIGINT, sigint_process_handler);
	signal(SIGQUIT, sigquit_process_handler);
}
