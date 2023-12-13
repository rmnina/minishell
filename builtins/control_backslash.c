/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control_backslash.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 15:13:39 by juandrie          #+#    #+#             */
/*   Updated: 2023/12/12 23:02:18 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_sigactionsq(struct sigaction *sq)
{
	// sq->sa_handler = SIG_IGN;
	// sigemptyset(&sq->sa_mask);
	sq->sa_flags = 0;
	if (sigaction(SIGQUIT, sq, NULL) < 0)
	{
		perror("SIGQUIT");
		return (-1);
	}
	return (0);
}


