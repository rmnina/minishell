/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 09:11:51 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/14 11:48:21 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    export()
{
    int i;

    i = 0;
    while(environ[i] != NULL)
    {
        printf("declare -x %s\n", environ[i]);
        i++;
    }
}

int main(int ac, char **av)
{
    if (ac > 1)
        export(av[1]);
    return(0);
}