/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:34:03 by lorey             #+#    #+#             */
/*   Updated: 2025/01/22 03:31:17 by lorey            ###   LAUSANNE.ch       */
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

int	find_index(t_env_data *e_data, char *var_name, bool is_set)
{
	int		i;
	size_t	var_name_len;

	var_name_len = ft_strlen(var_name);
	i = -1;
	if (!(is_valid_var_name(var_name)))
		return (write_env_error(var_name, "var (env) name is invalid "), -1);
	while (e_data->env[++i])
		if (ft_strncmp(e_data->env[i], var_name, var_name_len) == 0)
			return (i);
	if (is_set == false)
		write_env_error(var_name, "cannot find variable ");
	else
		write_env_error(var_name, "creating variable ");
	return (-2);
}

char	*get_env(t_env_data *e_data, char *var_name)
{
	int		i;
	char	*value_start;

	i = find_index(e_data, var_name, false);
	if (i < 0)
		return (NULL);
	value_start = ft_strchr(e_data->env[i], '=');
	if (value_start == NULL)
		return (write_env_error(var_name, "variable exists but no '=' "), NULL);
	if (value_start[1] != 0)
		return (ft_strdup(value_start + 1));
	return (NULL);
}

/*
void	set_env2(t_env_data *e_data, char *var_name, char *value)
{
	int	i;
	int	j;
	int	k;

	i = find_index(e_data, var_name);
	if (i < 0)
		return ;
	free(e_data->env[i]);
	e_data->env[i] = malloc(sizeof(char)
			* (ft_strlen(var_name) + ft_strlen(value) + 2));
	j = -1;
	while (var_name[++j])
		e_data->env[i][j] = var_name[j];
	var_name[j] = '=';
	k = -1;
	while (value[++k])
		e_data->env[i][++j] = value[k];
	e_data->env[i][j] = '\0';
}
*/

void	set_env(t_env_data *e_data, char *var_name, char *value)
{
	int		i;
	int		count;
	char	**new_env;

	i = find_index(e_data, var_name, true);
	if (i >= 0)
	{
		e_data->env[i] = ft_strjoin(var_name, "=");
		e_data->env[i] = ft_strjoin(e_data->env[i], value);
	}
	else if (i == -2)
	{
		count = 0;
		while (e_data->env[count])
			count++;
		new_env = malloc(sizeof(char *) * (count + 2));
		if (!new_env)
			return ;
		for (int j = 0; j < count; j++)
			new_env[j] = e_data->env[j];
		new_env[count] = ft_strjoin(var_name, "=");
		new_env[count] = ft_strjoin(new_env[count], value);
		new_env[count + 1] = NULL;
		e_data->env = new_env;
	}
}
