/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:38:14 by lorey             #+#    #+#             */
/*   Updated: 2025/03/27 22:56:40 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

// Options:
//   -f		refer to shell functions
//   -n		remove the export property from each NAME
//   -p		display a list of all exported variables and functions

#include "minishell.h"

static void	put_var_without_equal_in_env(
	t_var *v_data, char *var, t_data *data)
{
	while (v_data)
	{
		if (ft_isequal(v_data->var_name, var))
		{
			set_env(v_data->var_name, v_data->var_value, true, data);
			return ;
		}
		v_data = v_data->next;
	}
	set_env(var, NULL, false, data);
}

static int	extract_var_name_value(
	char *input, char **var_name, char **var_value, t_data *data)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != '=')
		i++;
	if (input[i] != '=')
		return (0);
	*var_name = safe_malloc(i + 1, data);
	ft_strlcpy(*var_name, input, i + 1);
	*var_value = ft_strdup(input + i + 1);
	if (*var_value == NULL)
	{
		safe_free((void **)&var_name);
		return (0);
	}
	return (1);
}

static int	put_var_with_equal_in_env(char *arg, t_data *data)
{
	char	*var_name;
	char	*var_value;

	if (!extract_var_name_value(arg, &var_name, &var_value, data))
	{
		write(1, "error extracting var name/value\n", 32);
		return (-1);
	}
	if (isalpha(arg[0]) || (arg[0] == '_'))
		set_env(var_name, var_value, true, data);
	else
		return (write(1, "invalid var name\n", 18), -1);
	safe_free((void **)&var_name);
	safe_free((void **)&var_value);
	return (0);
}

static int	export_with_arg(t_parsing_data *p_data,
			t_var *v_data, t_data *data, int i)
{
	if (ft_strchr(p_data->arg[i], '='))
	{
		if (isalpha(p_data->arg[i][0]) || (p_data->arg[i][0] == '_'))
		{
			if (put_var_with_equal_in_env(p_data->arg[i], data))
				return (p_data->status = 1, 1);
		}
		else
			return (p_data->status = 1
				, write(1, "invalid var name \n", 18), 1);
	}
	else if (isalpha(p_data->arg[i][0]) || (p_data->arg[i][0] == '_'))
		put_var_without_equal_in_env(v_data, p_data->arg[i], data);
	else
		return (p_data->status = 1
			, write(1, "invalid var name \n", 18), 1);
	return (0);
}

int	mini_export(t_parsing_data *p_data, t_path_data *path_data,
			t_var *v_data, t_data *data)
{
	int	i;

	p_data->status = 0;
	i = e_setup_flags(p_data, path_data);
	if (i == -2)
		return (p_data->status = 2, 2);
	if (p_data->arg && p_data->arg[i])
	{
		if (p_data->pipe)
			frexit(data, 0);
		i--;
		while (p_data->arg[++i])
		{
			if (export_with_arg(p_data, v_data, data, i))
				return (1);
		}
	}
	else
		copy_and_sort_array(data->env->env, p_data, data);
	return (0);
}
