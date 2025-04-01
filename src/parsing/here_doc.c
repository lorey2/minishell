/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lo>                                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 02:13:13 by lorey             #+#    #+#             */
/*   Updated: 2025/03/31 13:45:44 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*parse_quoted_delimiter(char **input, t_data *data, char *quote_char)
{
	int		len;
	char	*del;

	*quote_char = **input;
	(*input)++;
	len = 0;
	while ((*input)[len] && (*input)[len] != *quote_char)
		len++;
	del = safe_malloc(sizeof(char) * (len + 1), data);
	ft_strlcpy(del, *input, len + 1);
	*input += len;
	if (**input == *quote_char)
		(*input)++;
	return (del);
}

char	*parse_unquoted_delimiter(char **input, t_data *data)
{
	int		len;
	char	*del;

	len = 0;
	if ((*input[len] == '>' || *input[len] == '<'
			|| **input == '\0' || **input == '|'))
		return (NULL);
	while ((*input)[len] && (*input)[len] != ' ' && (*input)[len] != '>'
		&& (*input)[len] != '<' && (*input)[len] != '|')
		len++;
	del = safe_malloc(sizeof(char) * (len + 1), data);
	ft_strlcpy(del, *input, len + 1);
	*input += len;
	return (del);
}

char	*check_here_doc_del(char **input, t_data *data)
{
	char	*del;
	char	quote_char;

	skip_space(input);
	if (is_quote(**input))
		del = parse_quoted_delimiter(input, data, &quote_char);
	else
		del = parse_unquoted_delimiter(input, data);
	return (del);
}

t_here_docs	*get_last_here(t_here_docs *here)
{
	t_here_docs	*head;

	head = here;
	while (head)
	{
		if (!head->next)
			return (head);
		head = head->next;
	}
	return (head);
}

void	handle_here(char **input, t_parsing_data *pars, t_data *data)
{
	t_here_docs	*here;
	t_here_docs	*last_here;
	char		*delimiter;

	here = safe_malloc(sizeof(t_here_docs), data);
	init_new_here(here);
	delimiter = check_here_doc_del(input, data);
	pars->delimiter = delimiter;
	here->delimiter = delimiter;
	if (!pars->here_docs)
		pars->here_docs = here;
	else
	{
		last_here = get_last_here(pars->here_docs);
		last_here->next = here;
	}
	skip_space(input);
}
