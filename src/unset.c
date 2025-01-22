/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 23:08:54 by lorey             #+#    #+#             */
/*   Updated: 2025/01/21 23:11:15 by lorey            ###   LAUSANNE.ch       */
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
	int	arg_it;

	i = -1;
	arg_it = -1;
	if (ft_isequal(p_data->arg[0], "-v"))
		arg_it++;
	while (data->env->env[++i])
		write(1, data->env->env[i], ft_strlen(data->env->env[i]));
	write(1, "\n", 1);
}
