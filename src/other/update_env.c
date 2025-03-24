/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 16:34:03 by lorey             #+#    #+#             */
/*   Updated: 2025/03/24 14:51:05 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	find_index(t_env_data *e_data, char *var_name, bool is_set)
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

// 	if (value_start == NULL)
// 		return ("");
// //	return (write_env_error(var_name, "variable exists but no '=' "), NULL);
// 	if (value_start[1] != 0)

char	*get_env(t_env_data *e_data, char *var_name, t_var *var)
{
	int		i;
	char	*value_start;

	if (var)
	{
		if (var->var_name && ft_isequal(var_name, var->var_name))
			if (var->var_value)
				return (ft_strdup(var->var_value));
		while (var->next)
		{
			var = var->next;
			if (var->var_name && ft_isequal(var_name, var->var_name))
				if (var->var_value)
					return (ft_strdup(var->var_value));
		}
	}
	i = find_index(e_data, var_name, false);
	if (i < 0)
		return (ft_strdup(""));
	value_start = ft_strchr(e_data->env[i], '=');
	if (value_start == NULL)
		return (ft_strdup(""));
	if (value_start[1] != 0)
		return (ft_strdup(value_start + 1));
	return (ft_strdup(""));
}

char	*create_env_entry(char *var_name, char *value, bool is_equal)
{
	char	*entry;
	char	*temp;

	if (is_equal)
		entry = ft_strjoin(var_name, "=");
	else
		entry = ft_strdup(var_name);
	if (value != NULL)
	{
		temp = ft_strjoin(entry, value);
		safe_free((void **)&entry);
		entry = temp;
	}
	return (entry);
}

void	set_env(t_env_data *e_data, char *var_name, char *value, bool is_equal)
{
	int		i;
	int		count;
	char	**new_env;
	char	*temp;

	i = find_index(e_data, var_name, true);
	if (i >= 0)
	{
		temp = ft_strjoin(var_name, "=");
		e_data->env[i] = ft_strjoin(temp, value);
		safe_free((void **)&temp);
	}
	else
	{
		count = 0;
		while (e_data->env[count])
			count++;
		new_env = malloc(sizeof(char *) * (count + 2));
		if (!new_env)
			return ;
		i = -1;
		while (++i < count)
			new_env[i] = e_data->env[i];
		new_env[count] = create_env_entry(var_name, value, is_equal);
		new_env[count + 1] = NULL;
		free_double_point(&e_data->env);
		e_data->env = new_env;
	}
}
