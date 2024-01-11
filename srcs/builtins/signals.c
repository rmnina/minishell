/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:13:20 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/11 15:02:46 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	init_sigactionsa(struct sigaction *sa)
// {
// 	ft_bzero(sa, sizeof(struct sigaction));
// 	sa->sa_handler = sigint_handler;
//     sa->sa_flags = 0;
// 	sigaction(SIGINT, sa, NULL);
// 	signal(SIGQUIT, SIG_IGN);
// 	return (0);
// }

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

// int init_sigquit(void)
// {
// 	struct sigaction	quit;

// 	sigemptyset(&quit.sa_mask);
//  	quit.sa_handler = sigquit_handler;
//  	quit.sa_flags = 0;
//  	sigaction(SIGQUIT, &quit, NULL);
// 	//signal(SIGQUIT, SIG_IGN);
// 	return(0);
// }

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
	printf("Quit (core dumped)\n");;
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

// int	process_prompt(void)
// {
// 	struct sigaction	prompt;

// 	ft_bzero(&prompt, sizeof(prompt));
// 	prompt.sa_handler = child_handler;
//     prompt.sa_flags = 0;
// 	sigaction(SIGINT, &prompt, NULL);

// 	return (0);
// }

// void	sigint_handler(int signum)
// {	
// 	if (signum == SIGINT)
// 	{
// 		//printf("sigint_handler: Signal SIGINT reçu (signum=%d)\n", signum);
// 		//rl_replace_line("", 0);
// 		write(STDOUT_FILENO, "\n", 1);
// 		rl_on_new_line();
// 		rl_redisplay();
// 	}
// 	if (signum == SIGQUIT)
// 	{
// 		//printf("sigquit_handler: Signal SIGQUIT reçu (signum=%d)\n", signum);
// 		printf("Quit (core dumped)\n");
// 	}
// }


