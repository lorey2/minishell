/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 16:31:48 by lorey             #+#    #+#             */
/*   Updated: 2025/01/24 17:10:11 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

// Options:
//   -f		refer to shell functions
//   -n		remove the export property from each NAME
//   -p		display a list of all exported variables and functions

#include "minishell.h"

static int	setup_flags(t_parsing_data *p_data, t_path_data *path_data)
{
	int	i;

	i = 0;
	init_flags(path_data);
	while (p_data->arg[++i] && p_data->arg[i][1] && p_data->arg[i][0] == '-' && \
		does_contain_only(p_data->arg[i], "fnp"))
		fill(p_data->arg[i], path_data);
	if (path_data->is_f || path_data->is_n || path_data->is_p)
		return (write_err(" -f, -n and -p are not implemented yet :'(\n"), -1);
	return (i);
}

void	calculate_rows_and_cols(char **src, int *rows, int *cols)
{
	int	i;
	int	current_len;

	i = 0;
	*cols = 0;
	while (src[i] != NULL)
	{
		(*rows)++;
		current_len = strlen(src[i]);
		if (current_len > *cols)
			*cols = current_len;
		i++;
	}
}

void	bubble_sort(char **arr, int rows, int max_len)
{
	int		i;
	int		j;
	int		swapped;
	char	*temp;

	i = -1;
	while (++i < rows - 1)
	{
		j = -1;
		swapped = 0;
		while (++j < rows - 1 - i)
		{
			if (strncmp(arr[j], arr[j + 1], max_len) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
				swapped = 1;
			}
		}
		if (!swapped)
			break ;
	}
}

//DEST IS NOT FREED FOR NOW

void	copy_and_sort_array(char **src)
{
	int		rows;
	int		cols;
	int		i;
	char	**dest;

	rows = 0;
	cols = 0;
	calculate_rows_and_cols(src, &rows, &cols);
	dest = malloc(rows * sizeof(char *));
	if (dest == NULL)
		perror("Unable to allocate memory for destination array");
	i = -1;
	while (++i < rows)
	{
		dest[i] = malloc((cols + 1) * sizeof(char));
		if (dest[i] == NULL)
			perror("Unable to allocate memory for destination row");
		ft_strlcpy(dest[i], src[i], cols + 1);
	}
	bubble_sort(dest, rows, cols);
	i = -1;
	while (dest[++i])
	{
		write(1, "declare -x ", ft_strlen("declare -x "));
		write(1, dest[i], ft_strlen(dest[i]));
		write(1, "\n", 1);
	}
}

int	mini_export(t_parsing_data *p_data, t_path_data *path_data,
			t_var *v_data, t_env_data *e_data)
{
	int		i;

	(void)v_data;
	i = setup_flags(p_data, path_data);
	if (p_data->arg[i])
	{
		i--;
		while (p_data->arg[++i])
		{
		}
	}
	else
		copy_and_sort_array(e_data->env);
	return (0);
}
