/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:33:13 by jdufour           #+#    #+#             */
/*   Updated: 2023/05/02 22:04:44 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(const char *s, char c)
{	
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == c || (i == 0 && s[i] != c))
		{
			if (s[i + 1] != c && s[i + 1])
				count++;
		}
		i++;
	}
	return (count);
}

static char	*ft_create_pieces(const char *s, char c)
{
	int		i;
	int		size;
	char	*piece;

	i = 0;
	size = 0;
	while (s[size] && s[size] != c)
		size++;
	piece = malloc(sizeof(char) * (size + 1));
	if (!(piece))
		return (NULL);
	while (i < size)
	{
		piece[i] = s[i];
		i++;
	}
	piece[i] = '\0';
	return (piece);
}

static void	ft_free(char **res)
{
	int	i;

	i = 0;
	while (res[i])
		i++;
	while (i >= 0 && res[i])
	{
		free(res[i]);
		i--;
	}
	free (res);
}

char	**ft_split(const char *s, char c)
{
	int		i;
	char	**res;

	i = 0;
	res = malloc(sizeof(char *) * ((ft_count(s, c)) + 1));
	if (!(res))
		return (NULL);
	while (*s)
	{
		if (*s != c && *s)
		{
			res[i] = ft_create_pieces(s, c);
			if (!(res[i]))
				ft_free(res);
			i++;
			while (*s != c && *s)
				s++;
		}
		while (*s == c && *s)
			s++;
	}
	res[i] = NULL;
	return (res);
}
/*
int main()
{
    char **res;
    
    res = ft_split("tripouille", ' ');
    for (int i = 0; i < 1; i++)
    {
        printf("%s\n", res[i]);
    }
}
*/
