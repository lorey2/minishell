/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   value.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:07:08 by lorey             #+#    #+#             */
/*   Updated: 2025/03/31 14:07:10 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_quoted_value(char **input, t_data *data)
{
	char	quote_char;
	int		len;
	char	*value;

	len = 0;
	quote_char = **input;
	(*input)++;
	while ((*input)[len] && (*input)[len] != quote_char)
	{
		len++;
	}
	value = safe_malloc(sizeof(char) * (len + 1), data);
	ft_strlcpy(value, *input, len + 1);
	*input += len;
	if (**input == quote_char)
		(*input)++;
	return (value);
}

char	*extract_unquoted_value(char **input, t_data *data)
{
	int		len;
	char	*value;

	len = 0;
	while ((*input)[len] && !check_meta_char((*input)[len]))
	{
		len++;
	}
	value = safe_malloc(sizeof(char) * (len + 1), data);
	ft_strlcpy(value, *input, len + 1);
	*input += len;
	return (value);
}

char	*get_value(char **input, t_data *data)
{
	if (is_quote(**input))
	{
		return (extract_quoted_value(input, data));
	}
	else
	{
		return (extract_unquoted_value(input, data));
	}
}
