/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   value.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:07:08 by lorey             #+#    #+#             */
/*   Updated: 2025/04/02 03:22:15 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_value(char **input, char **to_set, t_data *data)
{
	int		len;
	char	*value;

	len = get_length(input);
	if (len == 0)
		return (0);
	value = extract_content(input, len, data);
	*to_set = ft_strdup(value);
	if (value)
		safe_free((void **)&value);
	*input += len;
	return (1);
}

void	get_value(char **input, char **to_set, t_data *data)
{
	skip_space(input);
	process_value(input, to_set, data);
}
