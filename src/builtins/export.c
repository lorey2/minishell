/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 17:38:14 by lorey             #+#    #+#             */
/*   Updated: 2025/02/06 17:50:19 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

// Options:
//   -f		refer to shell functions
//   -n		remove the export property from each NAME
//   -p		display a list of all exported variables and functions

#include "minishell.h"

static int	setup_flags(t_parsing_data *p_data, t_path_data *path_data)
{
	int	i;

	i = 0;
	init_flags(path_data);
	while (p_data->arg[++i] && p_data->arg[i][1] && p_data->arg[i][0] == '-' && \
		does_contain_only(p_data->arg[i], "fnp"))
		fill(p_data->arg[i], path_data);
	if (path_data->is_f || path_data->is_n || path_data->is_p)
		return (write_err(" -f, -n and -p are not implemented yet :'(\n"), -1);
	return (i);
}

static void	put_var_without_equal_in_env(
	t_var *v_data, char *var, t_env_data *e_data)
{
	while (v_data)
	{
		if (ft_isequal(v_data->var_name, var))
		{
			set_env(e_data, v_data->var_name, v_data->var_value);
			break ;
		}
		v_data = v_data->next;
	}
}

static int	extract_var_name_value(
	char *input, char **var_name, char **var_value)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != '=')
		i++;
	if (input[i] != '=')
		return (0);
	*var_name = malloc(i + 1);
	if (*var_name == NULL)
		return (0);
	ft_strlcpy(*var_name, input, i + 1);
	*var_value = ft_strdup(input + i + 1);
	if (*var_value == NULL)
	{
		free(*var_name);
		return (0);
	}
	return (1);
}

static void	put_var_with_equal_in_env(char *arg, t_env_data *e_data)
{
	char	*var_name;
	char	*var_value;

	if (!extract_var_name_value(arg, &var_name, &var_value))
	{
		write(1, "error extracting var name/value\n", 32);
		return ;
	}
	if (isalpha(arg[0]) || (arg[0] == '_'))
		set_env(e_data, var_name, var_value);
	else
		write(1, "invalid var name\n", 18);
	free(var_name);
	free(var_value);
}

int	mini_export(t_parsing_data *p_data, t_path_data *path_data,
			t_var *v_data, t_env_data *e_data)
{
	int		i;

	i = setup_flags(p_data, path_data);
	if (p_data->arg[i])
	{
		i--;
		while (p_data->arg[++i])
		{
			if (ft_strchr(p_data->arg[i], '='))
			{
				if (isalpha(p_data->arg[i][0]) || (p_data->arg[i][0] == '_'))
					put_var_with_equal_in_env(p_data->arg[i], e_data);
				else
					write(1, "invalid var name \n", 18);
			}
			else if (isalpha(p_data->arg[i][0]) || (p_data->arg[i][0] == '_'))
				put_var_without_equal_in_env(v_data, p_data->arg[i], e_data);
			else
				write(1, "invalid var name \n", 18);
		}
	}
	else
		copy_and_sort_array(e_data->env);
	return (0);
}
