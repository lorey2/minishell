/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env_error.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:56:12 by lorey             #+#    #+#             */
/*   Updated: 2025/03/17 15:57:29 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_env_error(char *var_name, char *message)
{
	write_err(message);
	write(1, "[", 1);
	write(1, var_name, ft_strlen(var_name));
	write(1, "]", 1);
	write(1, "\n", 1);
}
