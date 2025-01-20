/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*													+:+ +:+		 +:+	 */
/*   By: lorey <lorey@student.42lausanne.ch>		+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2025/01/18 02:04:26 by lorey			 #+#	#+#			 */
/*   Updated: 2025/01/19 22:22:17 by lorey            ###   LAUSANNE.ch       */
/*																			*/
/* ************************************************************************** */

#include "minishell.h"

//like cd, pwd can have two flags -L and -P for links
//maybe we will implement that later but the setup is already here
//apart of that ther command does not care of the nbr of args

bool	contains_only_lp(char *to_verif)
{
	int	i;

	i = 0;
	while (to_verif[++i])
		if (to_verif[i] != 'L' && to_verif[i] != 'P')
			return (false);
	return (true);
}

bool	contains2(char *to_verif, char letter)
{
	int	i;

	i = 0;
	while (to_verif[++i])
		if (to_verif[i] == letter)
			return (true);
	return (false);
}

void	setup_args(t_parsing_data *p_data, bool *is_big_l, bool *is_big_p)
{
	int	i;

	i = 0;
	while (p_data->arg[++i])
	{
		if (p_data->arg[i][0] == '-')
		{
			if (contains_only_lp(p_data->arg[i]) == false)
				break ;
			if (contains2(p_data->arg[i], 'L'))
				*is_big_l = true;
			if (contains2(p_data->arg[i], 'P'))
				*is_big_p = true;
		}
		else
			break ;
	}
}

void	pwd(t_parsing_data *p_data)
{
	char	shell_prompt[1024];
	bool	is_big_l;
	bool	is_big_p;

	is_big_l = false;
	is_big_p = false;
	setup_args(p_data, &is_big_l, &is_big_l);
	if (is_big_l || is_big_p)
	{
		write(1, "-L and -P are not implemented yet :'(\n", 38);
	}
	else if (getcwd(shell_prompt, sizeof(shell_prompt)) == NULL)
	{
		write(1, "HOW CAN YOU HAVE NO CWD IN ENV\n", 31);
	}
	else
	{
		write(1, shell_prompt, ft_strlen(shell_prompt));
		write(1, "\n", 1);
	}
}
