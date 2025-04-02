/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:05:20 by lorey             #+#    #+#             */
/*   Updated: 2025/04/02 03:20:42 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_reset_quote(int *in_quote, char *quote_char, int i, char **input)
{
	if (!(*in_quote))
	{
		*quote_char = (*input)[i];
		*in_quote = 1;
		return (0);
	}
	else if ((*input)[i] == *quote_char)
	{
		*quote_char = 0;
		*in_quote = 0;
		return (0);
	}
	return (1);
}

char	*extract_content(char **input, int len, t_data *data)
{
	char	*arg;
	int		i;
	int		j;
	int		in_quote;
	char	quote_char;

	arg = safe_malloc(sizeof(char) * (len + 1), data);
	i = 0;
	j = 0;
	in_quote = 0;
	quote_char = 0;
	while (i < len)
	{
		if (is_quote((*input)[i]))
		{
			if (set_reset_quote(&in_quote, &quote_char, i, input))
				arg[j++] = (*input)[i];
		}
		else
			arg[j++] = (*input)[i];
		i++;
	}
	arg[j] = '\0';
	return (arg);
}

int	get_length(char **input)
{
	int		len;
	int		in_quote;
	char	quote_char;

	len = 0;
	in_quote = 0;
	quote_char = 0;
	while ((*input)[len])
	{
		if (is_quote((*input)[len]))
			set_reset_quote(&in_quote, &quote_char, len, input);
		else if (!in_quote
			&& ((*input)[len] == ' ' || check_meta_char_arg((*input)[len])))
			break ;
		len++;
	}
	return (len);
}

int	process_next_arg(char **input, t_parsing_data *pars,
						t_data *data, int *arg_index)
{
	int		len;
	char	*arg;

	len = get_length(input);
	if (len == 0)
		return (0);
	arg = extract_content(input, len, data);
	pars->arg[(*arg_index)++] = ft_strdup(arg);
	if (arg)
		safe_free((void **)&arg);
	*input += len;
	return (1);
}

void	get_arg(char **input, t_parsing_data *pars, t_data *data)
{
	int	arg_index;

	arg_index = 0;
	pars->arg = safe_malloc(sizeof(char *) * 1024, data);
	while (**input)
	{
		skip_space(input);
		if (!process_next_arg(input, pars, data, &arg_index))
			break ;
		if (**input == '\0')
			break ;
	}
	pars->arg[arg_index] = NULL;
	rebuild_arg(pars, data);
}
