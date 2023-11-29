/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/27 21:29:22 by jdufour           #+#    #+#             */
/*   Updated: 2023/11/29 17:02:55 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*create_expand_first_word(char *lexer, char *var, t_expand *expand)
{
	int		i;
	int		j;
	int		size;
	char	*command;

	i = 0;
	size = 0;
	j = 0;
	while (lexer[size] && lexer[size] != EXPAND)
		size++;
	while (var[i++] && var[i++] != SPACE)
		size++;
	command = malloc(sizeof(char) * (size + 1));
	if (!command)
		return (NULL);
		i = 0;
	while (lexer[i] != EXPAND)
		command[i++] = lexer[i++];
	if (lexer[i] == EXPAND)
	{
		while (var[j] && var[j] != SPACE)
			command[i++] = var[j++];
	}
	command[i] = '\0';
	expand->pos = j;
	return (command);
}

char	*get_expand_middle_words(char *var, t_expand *expand)
{
	int		i;
	int		j;
	int		size;
	char	*command;

	i = 0;
	j = expand->pos;
	size = 0;
	while (var[j++] != SPACE)
		size++;
	command = malloc(sizeof(char *) * size + 1);
	if (!command)
		return (NULL);
	j = expand->pos;
	while (var[j] != SPACE)
		command[i++] = var[j++];
	command[i] = '\0';
	expand->pos = j;
	return (command);
}

char	*get_expand_last_word(char *lexer, char *var, t_expand *expand)
{
	int		i;
	int		j;
	int		k;
	int		size;
	char	*command;

	i = 0;
	j = expand->pos;
	k = 0;
	size = 0;
	while (var[j++] != '\0')
		size++;
	while (lexer[i++] != EXPAND)
		size++;
	i++;
	command = malloc(sizeof(char) * size + 1);
	j = expand->pos;
	while (var[j] != '\0')
		command[k++] = var[j++];
	while (lexer[i] != '\0')
		command[k++] = lexer[i++];
	command[k] = '\0';
	return (command);
}

char	*ft_expand(t_command *lexer, char *command, t_expand *expand)
{
	int		i;
	int		j;
	int		size;
	char	*var;

	i = 0;
	j = 0;
	var = get_env(get_var_name(lexer, i, var));
	size = expand_size(var) + ;
	while ()
	
}

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