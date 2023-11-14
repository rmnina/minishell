/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 13:04:36 by juandrie          #+#    #+#             */
/*   Updated: 2023/11/14 13:14:40 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exit_shell(const char *exit_code_str)
{
    int exit_code;
    
    exit_code = 0;

    // Si un argument est fourni, convertissez-le en entier pour l'utiliser comme code de sortie.
    if (exit_code_str != NULL)
        exit_code = atoi(exit_code_str);
    // printf("Exiting shell with code %d\n", exit_code);
    exit(exit_code);
}

int main(int argc, char **argv) 
{
    if (argc > 1) 
        exit_shell(argv[1]);
    return 0;
}