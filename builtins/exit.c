/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:04:36 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/14 16:55:35 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_exit(const char *exit_code_str)
{
    int status;
    
    status = 0;

    // Si un argument est fourni, convertissez-le en entier pour l'utiliser comme code de sortie.
    if (exit_code_str != NULL)
        status = atoi(exit_code_str);
    printf("Exiting with code %d\n", status);
    exit(status);
}



int main(int argc, char **argv) 
{
    if (argc > 1 && strcmp(argv[1], "ft_exit") == 0) 
    {
        if (argc > 2)
            ft_exit(argv[2]);
        else
            ft_exit(NULL);
    }
    return (0);
}