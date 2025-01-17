/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 18:21:16 by lorey             #+#    #+#             */
/*   Updated: 2025/01/17 16:17:19 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_path(t_path_data *path_data)
{
	char	*path;

	path = getenv("PATH");
	path_data->path_split = ft_split(path, ':');
	if (!path_data)
		error("Malloc error", NULL);
}

char	*setup_prompt(t_data *data)
{
	int		i;
	char	*shell_prompt;

	shell_prompt = malloc(1024 * sizeof(char));
	if (!shell_prompt)
		error("malloc error", data);
	if (getcwd(shell_prompt, 1024) == NULL)
		error("getcwd", data);
	i = -1;
	while (shell_prompt[++i])
		;
	shell_prompt[i] = '$';
	shell_prompt[i + 1] = ' ';
	shell_prompt[i + 2] = '\0';
	return (shell_prompt);
}

void	setup_env(t_data *data, char **env)
{
	data->env->env = env;
}
