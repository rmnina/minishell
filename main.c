/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:45:11 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/14 17:06:19 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv)
{
	char	*line;
	char	*prompt;

	while (1)
	{
		prompt = "minishell > ";
		line = readline(prompt);
		printf("%s\n", line);
	} 
	// penser a la condition de sortie 
	//gerer les entrees speciales comme CTRL+C ou CTRL+D
	//penser a liberer la ligne renvoyee par readline 
	
}
