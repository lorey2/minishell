/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:04:01 by lorey             #+#    #+#             */
/*   Updated: 2025/03/31 14:04:13 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	rebuild_arg(t_parsing_data *pars, t_data *data)
{
	int		i;
	int		j;
	char	**arg_with_cmd;

	i = 0;
	j = 0;
	while (pars->arg[i])
		i++;
	arg_with_cmd = safe_malloc(sizeof(char *) * (i + 2), data);
	arg_with_cmd[j++] = ft_strdup(pars->value);
	i = 0;
	while (pars->arg[i])
		arg_with_cmd[j++] = ft_strdup(pars->arg[i++]);
	arg_with_cmd[j] = NULL;
	free_double_point(&pars->arg);
	pars->arg = arg_with_cmd;
}
