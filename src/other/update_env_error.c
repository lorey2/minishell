/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env_error.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:56:12 by lorey             #+#    #+#             */
/*   Updated: 2025/03/27 16:35:47 by lorey            ###   LAUSANNE.ch       */
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

bool	is_valid_var_name(char	*arg)
{
	int	i;

	i = -1;
	if (arg[0] == '\0')
		return (false);
	if (!(arg[0] == '_' || (arg[0] >= 'A' && arg[0] <= 'Z')
			|| (arg[0] >= 'a' && arg[0] <= 'z')))
		return (false);
	while (arg[++i])
		if (arg[i] == '=')
			return (false);
	return (true);
}
