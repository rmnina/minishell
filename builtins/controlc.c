/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controlc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:13:20 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/21 16:02:55 by jdufour          ###   ########.fr       */
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
    sa->sa_flags = SA_RESTART;
	sigaction(SIGINT, sa, NULL);

	return (0);
}

// void	sigint_child(int signum)
// {
	
// }

void	sigint_handler(int signum)
{

	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		printf("getpid = %d\n", getpid());
			//rl_redisplay();
		g_sigint = 1;
	}
	else if (signum == SIGQUIT)
	{
		perror("SIGQUIT");
		// return (-1);
	}
}

