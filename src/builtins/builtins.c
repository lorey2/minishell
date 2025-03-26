/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:03:28 by lorey             #+#    #+#             */
/*   Updated: 2025/03/26 18:06:22 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Cf message 4 fevrier sur whatsapp a marc c'est pas juste.
// Ce sera fait durant l'expand

bool	handle_dollar(t_data *data, t_parsing_data *p_data)
{
	if (p_data->value[1] == '?')
	{
		printf("%d%s", data->return_nbr, (p_data->value += 2));
	}
	return (false);
}

bool	is_builtin(char *cmd)
{
	return (ft_isequal(cmd, "echo")
		|| ft_isequal(cmd, "cd")
		|| ft_isequal(cmd, "pwd")
		|| ft_isequal(cmd, "export")
		|| ft_isequal(cmd, "unset")
		|| ft_isequal(cmd, "env")
		|| ft_isequal(cmd, "exit"));
}

bool	exec_builtin(t_data *data, t_parsing_data *p_data)
{
	if (ft_isequal(p_data->value, "echo"))
		return (echo(p_data, data->path, data), true);
	else if (ft_isequal(p_data->value, "cd"))
		return (cd(p_data, data->path, data->env), true);
	else if (ft_isequal(p_data->value, "pwd"))
		return (pwd(p_data, data->path, data), true);
	else if (ft_isequal(p_data->value, "export"))
		return (mini_export(p_data, data->path, data->var, data), true);
	else if (ft_isequal(p_data->value, "unset"))
		return (unset(data->env, data->path, p_data), true);
	else if (ft_isequal(p_data->value, "env"))
		return (env(data->env, p_data, data), true);
	else if (ft_isequal(p_data->value, "exit"))
		return (mini_exit(data, p_data), true);
	else if (p_data->value[0] == '$')
		return (handle_dollar(data, p_data));
	return (false);
}

bool	check_builtin(t_data *data, t_parsing_data *p_data)
{
	if (!is_builtin(p_data->value))
		return (false);
	return (exec_builtin(data, p_data));
}
