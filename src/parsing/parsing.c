/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 21:15:06 by lorey             #+#    #+#             */
/*   Updated: 2025/03/29 13:02:39 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO error gestion

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
	return (c == '|' || c == '&' || c == ';' || c == '(' || c == ')'
		|| c == '<' || c == '>' || c == ' ');
}

int	check_meta_char_arg(char c)
{
	return (c == '|' || c == '&' || c == ';' || c == '(' || c == ')'
		|| c == '<' || c == '>');
}

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

char	*get_value(char **input, t_parsing_data *pars, int make_offset, t_data *data)
{
	(void)pars;
	(void)make_offset;
	if (is_quote(**input))
	{
		return (extract_quoted_value(input, data));
	}
	else
	{
		return (extract_unquoted_value(input, data));
	}
}

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

char	*extract_argument(char **input, int len, t_data *data)
{
	char	*arg;
	int		i;
	int		j;
	int		in_quote;
	char	quote_char;

	i = 0;
	j = 0;
	arg = safe_malloc(sizeof(char) * (len + 1), data);
	in_quote = 0;
	quote_char = 0;
	while (i < len)
	{
		if (is_quote((*input)[i]))
		{
			if (!in_quote)
			{
				quote_char = (*input)[i];
				in_quote = 1;
			}
			else if ((*input)[i] == quote_char)
			{
				quote_char = 0;
				in_quote = 0;
			}
			i++;
			continue ;
		}
		arg[j++] = (*input)[i++];
	}
	arg[j] = '\0';
	return (arg);
}

int	set_reset_quote(int *in_quote, char *quote_char, int i, char **input)
{
	if (!(*in_quote))
	{
		*quote_char = (*input)[i];
		*in_quote = 1;
		return (0);
	}
	//si oui et qu'il y a eut un quote avant on reset
	else if ((*input)[i] == *quote_char)
	{
		*quote_char = 0;
		*in_quote = 0;
		return (0);
	}
	return (1);
}

void	get_arg(char **input, t_parsing_data *pars, t_data *data)
{
	int		len;
	char	*arg;
	char	quote_char;
	int		in_quote;
	int		arg_index;
	int		i;
	int		j;

	arg_index = 0;
	pars->arg = safe_malloc(sizeof(char *) * 1024, data);
	while (**input)
	{
		skip_space(input);
		len = 0;
		in_quote = 0;
		quote_char = 0;
	// ici on calcule la longueur du prochain arg pour le malloc
		while ((*input)[len])
		{
			if (is_quote((*input)[len]))
				set_reset_quote(&in_quote, &quote_char, len, input);
			else if (!in_quote
				&& ((*input)[len] == ' ' || check_meta_char_arg((*input)[len])))
				break ;
			len++;
		}
		if (len == 0)
			break ;
		arg = safe_malloc(sizeof(char) * (len + 1), data);
		i = 0;
		j = 0;
		in_quote = 0;
		quote_char = 0;
		//et donc ici on va allouer arg
		while (i < len)
		{
			//pareil on check si le charac est un quote
			if (is_quote((*input)[i]))
			{
				if (set_reset_quote(&in_quote, &quote_char, i, input))
					arg[j++] = (*input)[i];
			}
			//si c'est pas un quote on reflechit pas et on le met dans arg
			else
				arg[j++] = (*input)[i];
			i++;
		}
		arg[j] = '\0';
		pars->arg[arg_index++] = ft_strdup(arg);
		if (arg)
			safe_free((void **)&arg);
		//ici on decale le pointeur
		*input += len;
		if (**input == '\0')
			break ;
	}
	pars->arg[arg_index] = NULL;
	rebuild_arg(pars, data);
}

char	*check_here_doc_del(char **input, t_data *data)
{
	int		len;
	char	*del;
	char	quote_char;

	skip_space(input);
	if (is_quote(**input))
	{
		quote_char = **input;
		(*input)++;
		len = 0;
		while ((*input)[len] && (*input)[len] != quote_char)
			len++;
		del = safe_malloc(sizeof(char) * (len + 1), data);
		ft_strlcpy(del, *input, len + 1);
		*input += len;
		if (**input == quote_char)
			(*input)++;
	}
	else
	{
		len = 0;
		if ((*input[len] == '>' || *input[len] == '<'
				|| **input == '\0' || **input == '|'))
			return (NULL);
		while ((*input)[len] && (*input)[len] != ' '
				&& (*input)[len] != '>' && (*input)[len] != '<')
			len++;
		del = safe_malloc(sizeof(char) * (len + 1), data);
		ft_strlcpy(del, *input, len + 1);
		*input += len;
	}
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

t_file	*get_last_file(t_file *file)
{
	t_file	*head;

	head = file;
	while (head)
	{
		if (!head->next)
			return (head);
		head = head->next;
	}
	return (head);
}

void	allocate_file(char **input, t_parsing_data *pars, t_data *data)
{
	t_file	*file;
	t_file	*last_file;

	file = safe_malloc(sizeof(t_file), data);
	init_new_file(file);
	pars->infile = get_value(input, pars, 1, data);
	file->name = pars->infile;
	if (!pars->infile_list)
		pars->infile_list = file;
	else
	{
		last_file = get_last_file(pars->infile_list);
		last_file->next = file;
	}
	skip_space(input);
}

void	handle_in_file(char **input, t_parsing_data *pars, t_data *data)
{
	++(*input);
	pars->in_file = true;
	if (**input == '<')
	{
		pars->infile = NULL;
		++(*input);
		handle_here(input, pars, data);
	}
	else
	{
		skip_space(input);
		allocate_file(input, pars, data);
		pars->delimiter = NULL;
	}
	skip_space(input);
}

void	handle_out_file(char **input, t_parsing_data *pars, t_data *data)
{
	t_file	*file;
	t_file	*last_file;

	file = safe_malloc(sizeof(t_file), data);
	init_new_file(file);
	pars->out_file = true;
	++(*input);
	if (**input == '>')
	{
		pars->append_file = true;
		file->append = true;
		++(*input);
	}
	skip_space(input);
	pars->outfile = get_value(input, pars, 1, data);
	file->name = pars->outfile;
	if (!pars->outfile_list)
		pars->outfile_list = file;
	else
	{
		last_file = get_last_file(pars->outfile_list);
		last_file->next = file;
	}
	skip_space(input);
}

int	check_for_file(char **input, t_parsing_data *pars, t_data *data)
{
	if (**input == '<')
	{
		handle_in_file(input, pars, data);
		return (1);
	}
	else if (**input == '>')
	{
		handle_out_file(input, pars, data);
		return (1);
	}
	return (0);
}

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

int	handle_cmd(char **input, t_parsing_data *pars, t_data *data)
{
	(*input) += check_vars_count(input);
	pars->is_cmd = true;
	skip_space(input);
	pars->value = get_value(input, pars, 0, data);
	skip_space(input);
	while (check_for_file(input, pars, data))
		;
	get_arg(input, pars, data);
	while (check_for_file(input, pars, data))
		;
	skip_space(input);
	if (**input == '|')
	{
		(*input)++;
		skip_space(input);
		if ((**input == '|' || **input == '\0') && !pars->here_docs)
			return (ft_putstr_fd("syntax error near unexpected token `|'\n"
					, STDERR_FILENO), 0);
		pars->pipe = true;
	}
	return (1);
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

t_var	*get_last(t_var *var)
{
	t_var	*head;

	head = var;
	while (head)
	{
		if (!head->next)
			return (head);
		head = head->next;
	}
	return (head);
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
	{
		var->var_value[i++] = **input;
		(*input)++;
	}
	var->var_value[i] = '\0';
	if (!data->var)
		data->var = var;
	else
	{
		last_var = get_last(data->var);
		last_var->next = var;
	}
}

int	parsing(char *input, t_data *data)
{
	t_parsing_data	*pars;
	t_parsing_data	*prev;
	int				pos;
	int				remove_offset;

	prev = NULL;
	pos = 0;
	while (*input)
	{
		while (check_vars_count(&input) > 0)
			add_var(&input, data);
		if (*input == '\0')
		{
			data->token = NULL;
			return (1);
		}
		remove_offset = check_vars_count(&input);
		input += (remove_offset * -1);
		pars = safe_malloc(sizeof(t_parsing_data), data);
		pars->previous = prev;
		init_new_token(pars);
		if (!prev)
			data->token = pars;
		if (prev)
			prev->next = pars;
		prev = pars;
		while (check_for_file(&input, pars, data))
			;
		if (!handle_cmd(&input, pars, data))
			return (0);
		pars->pos = pos;
		pos++;
	}
	pars->next = NULL;
	return (1);
}
