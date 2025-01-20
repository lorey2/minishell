/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 18:21:16 by lorey             #+#    #+#             */
/*   Updated: 2025/01/20 17:48:01 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	setup_path(t_path_data *path_data)
{
	char	*path;
	int		i;

	path_data->pwd = getenv("PWD");
	path_data->oldpwd = NULL;
	path = getenv("PATH");
	if (!path)
		error("PATH environment variable not found", NULL);
	path_data->path_split = ft_split(path, ':');
	if (!path_data->path_split)
		error("Malloc error for path_split", NULL);
	i = 0;
	while (path_data->path_split[i])
		i++;
	path_data->path_split_slash = malloc((i + 1) * sizeof(char *));
	if (!path_data->path_split_slash)
		error("Malloc error for path_split_slash", NULL);
	i = -1;
	while (path_data->path_split[++i])
	{
		path_data->path_split_slash[i]
			= ft_strjoin(path_data->path_split[i], "/");
		if (!path_data->path_split_slash[i])
			error("Malloc error for path with slash", NULL);
	}
	path_data->path_split_slash[i] = NULL;
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
