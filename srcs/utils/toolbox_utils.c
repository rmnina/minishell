/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toolbox_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 05:26:30 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/28 20:01:00 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_count(t_command *command, int *i)
{
	int	size;

	size = 0;
	while (command[*i + size].type && command[*i + size].type == WORD)
		size++;
	return (size);
}

t_command	token_null(t_command *token, t_alloc **garbage)
{
	token->word = garb_malloc(sizeof(char), 1, PARSING, garbage);
	token->word[0] = '\0';
	token->type = 0;
	return (*token);
}

void	check_spaces(t_minishell **main, int *i)
{
	if ((*main)->line[*i] == 32)
	{
		while ((*main)->line[*i] && (*main)->line[*i] == 32)
			(*i)++;
	}
}

int	ft_strcmp_var(const char *s1, const char *s2)
{
	int				i;
	unsigned char	*str1;
	unsigned char	*str2;

	i = 0;
	str1 = (unsigned char *)s1;
	str2 = (unsigned char *)s2;
	while (str1[i] && str2[i] && str1[i] == str2[i])
	{
		if (str1[i] && str2[i] && str1[i] == '=' && str2[i] == '=')
			return (0);
		i++;
	}
	if (i == ft_strlen(s2) && str1[i] && str1[i] == '=' && str2[i] == '\0')
		return (0);
	else if (i == ft_strlen(s1) && str2[i] && str2[i] == '=' && str1[i] == '\0')
		return (0);
	return (str1[i] - str2[i]);
}

int	is_builtin(char *command)
{
	if (ft_strcmp(command, "cd") == 0 \
		|| ft_strcmp(command, "echo") == 0 \
		|| ft_strcmp(command, "env") == 0 \
		|| ft_strcmp(command, "exit") == 0 \
		|| ft_strcmp(command, "export") == 0 \
		|| ft_strcmp(command, "pwd") == 0 \
		|| ft_strcmp(command, "unset") == 0)
	{
		return (1);
	}
	return (0);
}
