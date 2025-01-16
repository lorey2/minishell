/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:03:28 by lorey             #+#    #+#             */
/*   Updated: 2025/01/15 18:27:13 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cd(t_data *data)
{
	if (chdir(data->token->value) == -1)
		error("chdir failed", data);
}

void	pwd(t_data *data)
{
	char	shell_prompt[1024];

	if (getcwd(shell_prompt, 1024) == NULL)
		error("getcwd", data);
	write(1, shell_prompt, ft_strlen(shell_prompt));
}

void	export(t_data *data)
{
	
}
