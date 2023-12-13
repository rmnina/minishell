/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controlc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:13:20 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/12 23:00:22 by jdufour          ###   ########.fr       */
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
	// sigemptyset(&sa->sa_mask);
	sigaction(SIGINT, sa, NULL);

	return (0);
}

void	sigint_handler(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\nminishell > ", 13);
}
