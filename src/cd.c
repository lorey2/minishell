/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <lorey@student.42lausanne.ch>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/18 01:20:21 by lorey             #+#    #+#             */
/*   Updated: 2025/01/19 21:01:24 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
//
//		IT WORKS BUT OLDPWD is not set, arg system -LLPPP -L -L -P neither

static void	cd_error(char *message)
{
	write(1, message, ft_strlen(message));
}

// here we only handle cd -
// it does cd to the last directory in which we had cd

static int	only_dash(t_parsing_data *p_data)
{
	//char	*old_pwd;

	//old_pwd = getenv("OLDPWD");
	if (ft_isequal(p_data->arg[1], "-"))
	{
//		if (!old_pwd)
//			return (cd_error("cd : OLDPWD not set\n"), 1);
//		else if (p_data->arg[2])
//			return (cd_error("cd : too many arguments\n"), 1);
		if (chdir("-") == -1)
			return (cd_error("cd : No such file or directory\n"), 1);
	}
	return (0);
}

// here we handle -L -P and --

static int	check_dash(t_parsing_data *p_data, char *home)
{
	if (p_data->arg[1][0] == '-')
	{
		if (only_dash(p_data))
			return (1);
		else if (ft_isequal(p_data->arg[1], "-P")
			|| ft_isequal(p_data->arg[1], "-L"))
			return (cd_error(" -L and -P are not implemented yet :'(\n"), 1);
		else if (ft_isequal(p_data->arg[1], "--"))
		{
			if (!p_data->arg[2])
			{
				if (!home)
					cd_error("cd : WTF YOU HAVE NO HOME\n");
				else if (chdir(home) == -1)
					cd_error("cd : No such file or directory\n");
			}
			else if (p_data->arg[3])
				return (cd_error("cd : too many argumets\n"), 1);
			else if (chdir(p_data->arg[2]) == -1)
				return (cd_error("cd : No such file or directory\n"), 1);
		}
		else
			return (cd_error("cd : Illegal option (-L, -P, --, -)\n"), 1);
	}
	return (0);
}

void	cd(t_parsing_data *p_data)
{
	char	*home;

	home = getenv("HOME");
	if (p_data->arg[1])
	{
		if (check_dash(p_data, home))
			return ;
		if (p_data->arg[2])
		{
			cd_error("cd : too many arguments\n");
			return ;
		}
		else if (ft_isequal(p_data->arg[1], "~"))
		{
			if (!home)
				cd_error("cd : WTF YOU HAVE NO HOME\n");
			else if (chdir(home) == -1)
				cd_error("cd : No such file or directory\n");
		}
		else
			if (chdir(p_data->arg[1]) == -1)
				cd_error("cd : No such file or directory\n");
	}
	else
	{
		if (!home)
			cd_error("cd : WTF YOU HAVE NO HOME\n");
		else if (chdir(home) == -1)
			cd_error("cd : No such file or directory\n");
	}
}
