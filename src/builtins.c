/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 17:03:28 by lorey             #+#    #+#             */
/*   Updated: 2025/01/17 23:04:09 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

//cd accept only one arg and a list of flags
//if you give more than one arg the rest is ignored
//the list of flags and interesting things are
//	cd -
//		gives the previous directory witch is stored in env OLDPWD
//	cd ~
//		/home
//	cd -P
//		forces the shell to follow symbolic links to their physical location
//	cd -L
//		forces the shell to follow symbolic links and navigate through them.
//		This is often the default behavior
//	cd -- -dir
//		with this flag we can now take dir starting with -

void	cd_error(char *message)
{
	write(1, message, ft_strlen(message));
}

int	check_dash(t_parsing_data *p_data)
{
	if (p_data->arg[1][0] == '-')
	{
		if (ft_isequal(p_data->arg[1], "-"))
		{
			if (p_data->arg[2])
				return (cd_error("cd : too many arguments"), 1);
			if (chdir(getenv(getenv("OLDPWD"))) == -1)
				return (cd_error("cd : No such file or directory"), 1);
		}
		else if (ft_isequal(p_data->arg[1], "-P")
			|| ft_isequal(p_data->arg[1], "-L"))
			return (cd_error("not implemented yet :'("), 1);
		else if (ft_isequal(p_data->arg[1], "--"))
		{
			if (p_data->arg[3])
				return (cd_error("cd : too many argumets"), 1);
			if (chdir(p_data->arg[2]) == -1)
				return (cd_error("cd : No such file or directory"), 1);
		}
		else
			return (cd_error("cd : Illegal option"), 1);
	}
	return (0);
}

void	cd(t_data *data, t_parsing_data *p_data)
{
	if (check_dash)
		return ;
	else if (ft_isequal(p_data->arg[1], "~"))
		chdir(getenv("HOME"));
	else
	{
		if (chdir(data->token->value) == -1)
			error("chdir failed", data);
	}
}

void	pwd(t_data *data)
{
	char	shell_prompt[1024];

	if (getcwd(shell_prompt, 1024) == NULL)
		error("getcwd", data);
	write(1, shell_prompt, ft_strlen(shell_prompt));
}

//unset has two flags possible
//-v to remove a variable from env
//-f to remove a function from shell environement
//no flag and -v is the same
//unset VAR1 VAR2 is possible
//unset with no flags and no args will do nothing
//IDK IF WE WILL HANDLE -f

void	unset(t_data *data, t_parsing_data *p_data)
{
	int	i;

	(void)p_data;
//	int	arg_it;

	i = -1;
//	arg_it = -1;
//	if (ft_isequal(p_data.arg[0], "-v"))
//		arg_it++;
//	while(p_data.arg[++arg_it])
//	{
	while (data->env->env[++i])
		write(1, data->env->env[i], ft_strlen(data->env->env[i]));
	write(1, "\n", 1);
}
