/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 02:14:39 by lorey             #+#    #+#             */
/*   Updated: 2025/04/01 17:12:02 by maambuhl         ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

void	skip_space(char **input)
{
	while (**input == ' ' || **input == '\t')
		(*input)++;
}

int	check_meta_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ');
}

int	check_meta_char_arg(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
