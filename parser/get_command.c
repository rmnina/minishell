/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 21:29:22 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/29 13:24:14 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



char	*ft_expand(t_command *lexer, char **command, int *i, int *j)
{
	char	*var;
	int		k;
	int		l;

	k = 0;
	l = 0;
	var = get_env(get_var_name(lexer, i, var));
	if (expand_size(var) > 1)
	{
		while (var[k])
		{
			if (var[k] == SPACE)
			{
				while(var[k] && var[k] == SPACE)
					k++;
			}
			while (var[k] && lexer[*i].word[*j] != SPACE)
			{
				if (lexer[*i].word[*j] != EXPAND)
				{
					while (lexer[*i].word[*j] != EXPAND)
						command[k++] = lexer[*i].word[*j++];
				}
				
				if (lexer[*i].word[*j] == EXPAND)
					*j++;
				command[l] = var[k];
				l++;
				k++;
			}
		}
	}
	
}

char	**get_command(t_command *lexer, char *var)
{
	char	**command;
	int		i;
	int		j;

	command = malloc(sizeof(char *) * commande_size(lexer, var) + 1);
	if (!command)
		return (NULL);
	i = 0;
	j = 0;
	while (lexer[i].word)
	{
		if (lexer[i].type == EXPAND)
			ft_expand(lexer, command, &i, &j);
		else
			command[j] = ft_strjoin(command[j], lexer[i].word);
		i++;
		j++;
	}
	return (command);
}