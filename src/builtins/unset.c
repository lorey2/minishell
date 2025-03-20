/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 23:08:54 by lorey             #+#    #+#             */
/*   Updated: 2025/03/19 17:47:24 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//unset has two flags possible
//-v to remove a variable from env
//-f to remove a function from shell environement
//no flag and -v is the same
//unset VAR1 VAR2 is possible
//unset with no flags and no args will do nothing
//IDK IF WE WILL HANDLE -f

static int	setup_flags(t_parsing_data *p_data, t_path_data *path_data)
{
	int	i;

	i = 0;
	init_flags(path_data);
	while (p_data->arg[++i] && p_data->arg[i][1] && p_data->arg[i][0] == '-' && \
		does_contain_only(p_data->arg[i], "vf"))
		fill(p_data->arg[i], path_data);
	if (path_data->is_f || path_data->is_v)
		return (write_err("unset : -f and -v is not implemented yet :'(\n"), -1);
	return (i);
}

static int	find_index(t_env_data *e_data, char *var_name)
{
	int		i;
	size_t	var_name_len;

	var_name_len = ft_strlen(var_name);
	i = -1;
	while (e_data->env[++i])
		if (ft_strncmp(e_data->env[i], var_name, var_name_len) == 0)
			return (i);
	return (-2);
}

// MAYBE WE WILL HAVE TO FREE e_data->env[j]

int	unset(t_env_data *e_data, t_path_data *path_data, t_parsing_data *p_data)
{
	int	i;
	int	j;

	if (p_data->pipe)
		exit(0);
	i = setup_flags(p_data, path_data);
	if (i == -1)
		return (1);
	while (p_data->arg[i])
	{
		j = find_index(e_data, p_data->arg[i]);
		if (j >= 0)
		{
			e_data->env[j] = NULL;
			while (e_data->env[j] && e_data->env[j + 1])
			{
				e_data->env[j] = e_data->env[j + 1];
				j++;
			}
			e_data->env[j] = NULL;
		}
		i++;
	}
	return (0);
}
