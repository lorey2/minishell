/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:38:34 by lorey             #+#    #+#             */
/*   Updated: 2025/03/24 18:49:34 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

void	free_path(t_path_data *path_data)
{
	safe_free((void **)&path_data->env_path);
	free_double_point(&path_data->path_split);
	free_double_point(&path_data->path_split_slash);
	safe_free((void **)&path_data->path_with_com);
}

void	setup_path(t_data *data, t_path_data *path_data)
{
	int		i;

	free_path(path_data);
	path_data->env_path = get_env(data->env, "PATH", NULL);
	if (!path_data->env_path)
		error("PATH environment variable not found", NULL);
	path_data->path_split = ft_split(path_data->env_path, ':');
	if (!path_data->path_split)
		error("Malloc error for path_split", NULL);
	safe_free((void **)&path_data->env_path);
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
	free_double_point(&path_data->path_split);
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
	int	i;
	int	count;

	count = 0;
	while (env[count] != NULL)
		count++;
	data->env->env = malloc(sizeof(char *) * (count + 1));
	if (!data->env->env)
		return ;
	for (i = 0; i < count; i++)
	{
		data->env->env[i] = ft_strdup(env[i]);
		if (!data->env->env[i])
		{
			for (int j = 0; j < i; j++)
				safe_free((void **)&data->env->env[j]);
			safe_free((void **)&data->env->env);
			return ;
		}
	}
	data->env->env[count] = NULL;
}
