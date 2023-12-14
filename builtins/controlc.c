/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controlc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:13:20 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/14 18:08:56 by juandrie         ###   ########.fr       */
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
	sa->sa_handler = sigint_handler;
    sa->sa_flags = 0;
	sigaction(SIGINT, sa, NULL);

	return (0);
}

void	sigint_handler(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
}
