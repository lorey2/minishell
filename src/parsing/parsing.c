/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 14:23:58 by lorey             #+#    #+#             */
/*   Updated: 2025/03/19 18:11:32 by maambuhl         ###   LAUSANNE.ch       */
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
	if (c == '|' || c == '&' || c == ';' || c == '(' || c == ')'
		|| c == '<' || c == '>' || c == ' ')
		return (1);
	return (0);
}

int	check_meta_char_arg(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '(' || c == ')'
		|| c == '<' || c == '>')
		return (1);
	return (0);
}

char *get_value(char **input, t_parsing_data *pars, int make_offset)
{
    int     len;
    char    *value;
    char    quote_char;
    (void)pars;
	(void)make_offset;

    len = 0;
    if (is_quote(**input))
    {
        quote_char = **input;
        (*input)++;  // Skip opening quote
        while ((*input)[len] && (*input)[len] != quote_char)
            len++;
        value = malloc(sizeof(char) * (len + 1));
        if (!value)
            error("malloc error", NULL);
        ft_strlcpy(value, *input, len + 1);
        *input += len;
        if (**input == quote_char)
            (*input)++;  // Skip closing quote
    }
    else
    {
        while ((*input)[len] && !check_meta_char((*input)[len]))
            len++;
        value = malloc(sizeof(char) * (len + 1));
        if (!value)
            error("malloc error", NULL);
        if (!len)
            return (NULL);
        ft_strlcpy(value, *input, len + 1);
        *input += len;
    }
    return (value);
}

void	rebuild_arg(t_parsing_data *pars)
{
	int		i;
	int		j;
	char	**arg_with_cmd;

	i = 0;
	j = 0;
	while (pars->arg[i])
		i++;
	arg_with_cmd = malloc(sizeof(char *) * (i + 2));
	if (!arg_with_cmd)
    	error("malloc error", NULL);
	arg_with_cmd[j++] = ft_strdup(pars->value);
	i = 0;
	while (pars->arg[i])
		arg_with_cmd[j++] = ft_strdup(pars->arg[i++]);
	arg_with_cmd[j] = NULL;
	free_double_point(pars->arg);
	pars->arg = arg_with_cmd;
}

void    get_arg(char **input, t_parsing_data *pars)
{
    int     len;
    char    *arg;
    char    quote_char;
    int     in_quote;

    skip_space(input);
    len = 0;
    in_quote = 0;
    quote_char = 0;
    while ((*input)[len])
    {
        if (is_quote((*input)[len]))
        {
            if (!in_quote)
            {
                quote_char = (*input)[len];
                in_quote = 1;
            }
            else if ((*input)[len] == quote_char)
            {
                quote_char = 0;
                in_quote = 0;
            }
            len++;
            continue;
        }
        if (!in_quote && check_meta_char_arg((*input)[len]))
            break;
        len++;
    }
    
    if (len == 0)
        return;
        
    arg = malloc(sizeof(char) * (len + 1));
    if (!arg)
        error("malloc error", NULL);
    
    int i = 0;
    int j = 0;
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
            continue;
        }
        arg[j++] = (*input)[i++];
    }
    arg[j] = '\0';
    
    pars->arg = ft_split(arg, ' ');
    if (!pars->arg)
        error("malloc error", NULL);
    *input += len;
    rebuild_arg(pars);
	if (arg)
    	free(arg);
}

char    *check_here_doc_del(char **input)
{
    int     len;
    char    *del;
    char    quote_char;

    skip_space(input);
    if (is_quote(**input))
    {
        quote_char = **input;
        (*input)++;  // Skip opening quote
        len = 0;
        while ((*input)[len] && (*input)[len] != quote_char)
            len++;
        del = malloc(sizeof(char) * (len + 1));
        if (!del)
            error("malloc error", NULL);
        ft_strlcpy(del, *input, len + 1);
        *input += len;
        if (**input == quote_char)
            (*input)++;  // Skip closing quote
    }
    else
    {
        len = 0;
		if ((*input[len] == '>' || *input[len] == '<'))
			return (NULL);
        while ((*input)[len] && (*input)[len] != ' ' && (*input)[len] != '>' && (*input)[len] != '<')
            len++;
        del = malloc(sizeof(char) * (len + 1));
        if (!del)
            error("malloc error", NULL);
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

void	handle_here(char **input, t_parsing_data *pars)
{
	t_here_docs	*here;
	t_here_docs	*last_here;
	char		*delimiter;

	here = malloc(sizeof(t_here_docs));
	if (!here)
		error("malloc err", NULL);
	init_new_here(here);
	delimiter = check_here_doc_del(input);
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

void	handle_in_file(char **input, t_parsing_data *pars)
{
	++(*input);
	if (**input == '<')
	{
		++(*input);
		handle_here(input, pars);
	}
	else
	{
		pars->in_file = true;
		skip_space(input);
		pars->infile = get_value(input, pars, 1);
		pars->delimiter = NULL;
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

void	handle_out_file(char **input, t_parsing_data *pars)
{
	t_file	*file;
	t_file	*last_file;

	file = malloc(sizeof(t_file));
	if (!file)
		error("malloc error", NULL);
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
	pars->outfile = get_value(input, pars, 1);
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

int	check_for_file(char **input, t_parsing_data *pars)
{
	if (**input == '<')
	{
		handle_in_file(input, pars);
		return (1);
	}
	else if (**input == '>')
	{
		handle_out_file(input, pars);
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

void	handle_cmd(char **input, t_parsing_data *pars)
{
	(*input) += check_vars_count(input);
	pars->is_cmd = true;
	skip_space(input);
	pars->value = get_value(input, pars, 0);
	skip_space(input);
	while (check_for_file(input, pars))
		;
	get_arg(input, pars);
	while (check_for_file(input, pars))
		;
	skip_space(input);
	if (**input == '|')
	{
		pars->pipe = true;
		(*input)++;
	}
}

t_var	*allocate_var(char **input)
{
	t_var	*var;
	int		name_len;
	int		value_len;

	var = malloc(sizeof(t_var));
	init_new_var(var);
	if (!var)
		error("malloc error", NULL);
	name_len = 0;
	while ((*input)[name_len] != '=')
		name_len++;
	var->var_name = malloc(sizeof(char) * (name_len + 1));
	if (!var->var_name)
		error("malloc error", NULL);
	++name_len;
	value_len = 0;
	while (((*input)[name_len] != ' ' || (*input)[name_len] != '\t') && (*input)[name_len] != '\0')
	{
		name_len++;
		value_len++;
	}
	var->var_value = malloc(sizeof(value_len + 1));
	if (!var->var_value)
		error("malloc error", NULL);
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
	var = allocate_var(input);
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

void	parsing(char *input, t_data *data)
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
			return ;
		}
		remove_offset = check_vars_count(&input);
		input += (remove_offset * -1);
		pars = malloc(sizeof(t_parsing_data));
		pars->previous = prev;
		if (!pars)
			error("malloc error", data);
		if (!prev)
			data->token = pars;
		if (prev)
			prev->next = pars;
		prev = pars;
		init_new_token(pars);
		while (check_for_file(&input, pars))
			;
		handle_cmd(&input, pars);
		pars->pos = pos;
		pos++;
	}
	pars->next = NULL;
}

