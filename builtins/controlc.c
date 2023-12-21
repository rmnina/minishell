/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controlc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:13:20 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/21 18:55:00 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>


int	init_sigactionsa(struct sigaction *sa)
{
	if (!sa)
	{
		printf("Erreur : pointeur NULL vers struct sigactionsa.\n");
		return (-1);
	}
	ft_bzero(sa, sizeof(sa));
	sa->sa_handler = sigint_handler;
	//sigemptyset(&sa->sa_mask);
    sa->sa_flags = 0;
	sigaction(SIGINT, sa, NULL);

	return (0);
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
	else if (signum == SIGQUIT)
	{
		perror("SIGQUIT");
	}
}

