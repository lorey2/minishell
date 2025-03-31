/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 02:18:33 by lorey             #+#    #+#             */
/*   Updated: 2025/03/31 13:39:38 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_vars_count(char **input)
{
	int	i;
	int	j;
	int	found;

	i = 0;
	found = 0;
	skip_space(input);
	while ((*input)[i])
	{
		j = 0;
		while (ft_isalpha((*input)[i + j]) || (*input)[i + j] == '_')
			j++;
		if (((*input)[i + j] != '=' || !j) && !found)
			return (0);
		else if (((*input)[i + j] != '=') && found)
			return (i * -1);
		++i;
		i += j;
		while ((*input)[i] != ' ' && (*input)[i] != '\t' && (*input)[i])
			i++;
		while ((*input)[i] == ' ' || (*input)[i] == '\t')
			i++;
		found = 1;
	}
	return (i);
}

t_var	*allocate_var(char **input, t_data *data)
{
	t_var	*var;
	int		name_len;
	int		value_len;

	var = safe_malloc(sizeof(t_var), data);
	init_new_var(var);
	name_len = 0;
	while ((*input)[name_len] != '=')
		name_len++;
	var->var_name = safe_malloc(sizeof(char) * (name_len + 1), data);
	++name_len;
	value_len = 0;
	while (((*input)[name_len] != ' '
		|| (*input)[name_len] != '\t') && (*input)[name_len] != '\0')
	{
		name_len++;
		value_len++;
	}
	var->var_value = safe_malloc(sizeof(value_len + 1), data);
	return (var);
}

void	add_var(char **input, t_data *data)
{
	t_var	*var;
	t_var	*last_var;
	int		i;

	i = 0;
	skip_space(input);
	var = allocate_var(input, data);
	while (**input != '=')
	{
		var->var_name[i++] = **input;
		(*input)++;
	}
	(*input)++;
	var->var_name[i] = '\0';
	i = 0;
	while ((**input != ' ' && **input != '\t') && **input)
		var->var_value[i++] = *(*input)++;
	var->var_value[i] = '\0';
	if (!data->var)
		data->var = var;
	else
	{
		last_var = get_last(data->var);
		last_var->next = var;
	}
}
