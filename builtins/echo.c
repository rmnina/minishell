/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 16:06:16 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/14 18:36:05 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void echo(char **av)
{
    int i;
    int newline;

    i = 1;
    newline = 0;
    
    if(av[1] != NULL && strcmp(av[1], "-n") == 0)
    {
        newline = 1;
        i++;
    }
    while(av[i] != NULL)
    {
        printf("%s", av[i]);
        if(av[i + 1] != NULL)
            printf(" ");
        i++;
    }
    if (!newline)
        printf("\n");
}

int main(int ac, char **av)
{
    if (ac > 1)
        echo(av);
    else
        printf("\n");
    return(0);
}