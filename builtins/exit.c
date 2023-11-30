/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:04:36 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/30 16:50:48 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	my_exit(char **cmd_args)
{
	int	status;

	status = 0;
	if (cmd_args[1] != NULL)
		status = ft_atoi(cmd_args[1]);
	exit(status);
}

// int	main(int argc, char **argv)
// {
// 	if (argc > 1 && strcmp(argv[1], "my_exit") == 0)
// 	{
// 		if (argc > 2)
// 			my_exit(argv[2]);
// 		else
// 			my_exit(NULL);
// 	}
// 	return (0);
// }
