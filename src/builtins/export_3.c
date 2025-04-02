/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:52:05 by lorey             #+#    #+#             */
/*   Updated: 2025/04/02 01:56:27 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	e_setup_flags(t_parsing_data *p_data, t_path_data *path_data)
{
	int	i;

	i = 0;
	init_flags(path_data);
	if (!p_data->arg)
		return (i);
	while (p_data->arg[++i] && p_data->arg[i][0] == '-' && p_data->arg[i][1])
	{
		if (does_contain_only(p_data->arg[i], "fnp"))
			fill(p_data->arg[i], path_data);
		else
			return (write_err("bash: export: invalid option\n"), -2);
	}
	if (path_data->is_f || path_data->is_n || path_data->is_p)
		return (write_err(" -f, -n and -p are not implemented yet :'(\n"), -2);
	return (i);
}
