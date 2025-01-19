/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lorey@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 02:04:26 by lorey             #+#    #+#             */
/*   Updated: 2025/01/18 02:20:51 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//like cd, pwd can have two flags -L and -P for links
//maybe we will implement that later
//apart of that ther command does not care of the nbr of args

void	pwd(t_parsing_data *p_data)
{
	char	*pwd;

	pwd = getenv("PWD");
	if (ft_isequal(p_data->arg[1], "-L") || ft_isequal(p_data->arg[1], "-P"))
		write(1, "-L and -P are not implemented yet :'(\n", 38);
	if (pwd)
		write(1, pwd, ft_strlen(pwd));
	else
		write(1, "HOW CAN YOU HAVE NO CWD IN ENV", 30);
}

/*
void	pwd(t_parsing_data *p_data)
{
	char	shell_prompt[1024];

	if (ft_isequal(p_data->arg[1], "-L") || ft_isequal(p_data->arg[1], "-P"))
		write(1, "-L and -P are not implemented yet :'(\n", 38);
	if (getcwd(shell_prompt, 1024) == NULL)
		error("getcwd", data);
	write(1, shell_prompt, ft_strlen(shell_prompt));
}
*/
