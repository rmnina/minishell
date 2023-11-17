/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:45:11 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/16 22:31:55 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	*prompt;

	// printf("env: %s", envp[0]);
	while (1)
	{
		prompt = "minishell > ";
		line = readline(prompt);
		if (!line)
		{
			printf("exit\n");
			break;
		}
		if (line[0] != 0)
		{
			printf("%s\n", line);
			add_history(line);
		}
	}
	clear_history();
}
