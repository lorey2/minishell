/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:03:28 by lorey             #+#    #+#             */
/*   Updated: 2025/01/19 18:12:01 by lorey            ###   LAUSANNE.ch       */
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

void	unset(t_data *data, t_parsing_data *p_data)
{
	int	i;

	(void)p_data;
//	int	arg_it;

	i = -1;
//	arg_it = -1;
//	if (ft_isequal(p_data.arg[0], "-v"))
//		arg_it++;
//	while(p_data.arg[++arg_it])
//	{
	while (data->env->env[++i])
		write(1, data->env->env[i], ft_strlen(data->env->env[i]));
	write(1, "\n", 1);
}

bool	check_builtin(t_data *data, t_parsing_data *p_data)
{
	if (ft_isequal(p_data->value, "echo"))
		return (echo(p_data), true);
	else if (ft_isequal(p_data->value, "cd"))
		return (cd(p_data), true);
	else if (ft_isequal(p_data->value, "pwd"))
		return (pwd(p_data), true);
	else if (ft_isequal(p_data->value, "export"))
		return (write(1, "not yet implemented", 19), true);
	else if (ft_isequal(p_data->value, "unset"))
		return (unset(data, p_data), true);
	else if (ft_isequal(p_data->value, "env"))
		return (env(data->env), true);
	else if (ft_isequal(p_data->value, "exit"))
		return (write(1, "not yet implemented", 19), true);
	return (false);
}
