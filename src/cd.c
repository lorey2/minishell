/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:33:57 by lorey             #+#    #+#             */
/*   Updated: 2025/01/20 20:01:35 by lorey            ###   LAUSANNE.ch       */
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
//
//		IT WORKS BUT not arg system -LLPPP -L -L -P

static void	cd_error(char *message)
{
	write(1, message, ft_strlen(message));
}

int	do_cd_update_env(char *arg, t_path_data *path_data)
{
	char	cwd[1024];

	if (chdir(arg) == -1)
		return (cd_error("cd : No such file or directory\n"), 1);
	if (path_data->oldpwd != NULL)
		free(path_data->oldpwd);
	path_data->oldpwd = strdup(path_data->pwd);
	if (path_data->oldpwd == NULL)
		return (cd_error("cd : Memory allocation failed for oldpwd\n"), 1);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (cd_error("cd : getcwd failed\n"), 1);
	path_data->pwd = strdup(cwd);
	if (path_data->pwd == NULL)
		return (cd_error("cd : Memory allocation failed for pwd\n")
			, free(path_data->oldpwd), 1);
	return (0);
}

// here we only handle cd -
// it does cd to the last directory in which we had cd

static int	only_dash(t_parsing_data *p_data, t_path_data *path_data)
{
	if (ft_isequal(p_data->arg[1], "-"))
	{
		if (path_data->oldpwd == NULL)
			return (cd_error("cd : OLDPWD not set\n"), 1);
		else if (p_data->arg[2])
			return (cd_error("cd : too many arguments\n"), 1);
		write(1, path_data->oldpwd, ft_strlen(path_data->oldpwd));
		write(1, "\n", 1);
		return (do_cd_update_env(path_data->oldpwd, path_data), 1);
	}
	return (0);
}

// here we handle -L -P and --

static int	check_dash(t_parsing_data *p_data, t_path_data *path_data
		, char *home)
{
	if (p_data->arg[1][0] == '-')
	{
		if (only_dash(p_data, path_data) == 1)
			return (1);
		else if (ft_isequal(p_data->arg[1], "-P")
			|| ft_isequal(p_data->arg[1], "-L"))
			return (cd_error(" -L and -P are not implemented yet :'(\n"), 1);
		else if (ft_isequal(p_data->arg[1], "--"))
		{
			if (!p_data->arg[2])
			{
				if (!home)
					return (cd_error("cd : WTF YOU HAVE NO HOME\n"), 1);
				else
					do_cd_update_env(home, path_data);
			}
			else if (p_data->arg[3])
				return (cd_error("cd : too many argumets\n"), 1);
			else
				do_cd_update_env(p_data->arg[2], path_data);
		}
		else
			return (cd_error("cd : Illegal option (-L, -P, --, -)\n"), 1);
	}
	return (0);
}

int	cd(t_parsing_data *p_data, t_path_data *path_data)
{
	char	*home;

	home = getenv("HOME");
	if (p_data->arg[1])
	{
		if (check_dash(p_data, path_data, home))
			return (1);
		else if (p_data->arg[2])
			return (cd_error("cd : too many arguments\n"), 1);
		else if (ft_isequal(p_data->arg[1], "~"))
		{
			if (!home)
				return (cd_error("cd : HOME environment variable not set\n"), 1);
			return (do_cd_update_env(home, path_data), 0);
		}
		return (do_cd_update_env(p_data->arg[1], path_data), 0);
	}
	else
	{
		if (!home)
			return (cd_error("cd : HOME environment variable not set\n"), 1);
		return (do_cd_update_env(home, path_data), 0);
	}
}
