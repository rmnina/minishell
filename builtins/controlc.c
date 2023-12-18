/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controlc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:13:20 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/18 16:13:39 by juandrie         ###   ########.fr       */
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
	sigemptyset(&sa->sa_mask);
    sa->sa_flags = 0;
	sigaction(SIGINT, sa, NULL);

	return (0);
}

void	sigint_handler(int signum)
{
	(void)signum;;
	g_sigint = 1;
	//printf("Signal SIGINT reçu\n"); 
	rl_replace_line("", 0);
	//printf("rl_replace_line(\"\", 0) exécuté.\n");
	write(STDOUT_FILENO, "\n", 1);
	//printf("Nouvelle ligne affichée\n");
	rl_on_new_line();
	//printf("rl_on_new_line() exécuté.\n");
	rl_redisplay();
	//printf("rl_redisplay() exécuté.\n");

}

