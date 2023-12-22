/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controlc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:13:20 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/22 14:36:50 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>


int	init_sigactionsa(struct sigaction *sa)
{
	ft_bzero(sa, sizeof(sa));
	sa->sa_handler = sigint_handler;
	//sigemptyset(&sa->sa_mask);
    sa->sa_flags = 0;
	sigaction(SIGINT, sa, NULL);

	return (0);
}

void	sigquit_handler(int signum)
{
	if (signum == SIGQUIT)
	{
		printf("Quit (core dumped)\n");
	}
}
int init_sigquit(void)
{
	struct sigaction	quit;

	sigemptyset(&quit.sa_mask);
    quit.sa_handler = sigquit_handler;
    quit.sa_flags = 0;
    sigaction(, &quit, NULL);

	return(0);
}

void	child_handler(int signum)
{
	(void)signum;
	printf("\n");
}

int	process_prompt(void)
{
	struct sigaction	prompt;

	ft_bzero(&prompt, sizeof(prompt));
	prompt.sa_handler = child_handler;
    prompt.sa_flags = 0;
	sigaction(SIGINT, &prompt, NULL);

	return (0);
}

void	sigint_handler(int signum)
{	
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_redisplay();
	}
}

