/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 19:33:57 by lorey             #+#    #+#             */
/*   Updated: 2025/01/21 03:45:29 by lorey            ###   LAUSANNE.ch       */
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
//		WE WONT MAkE -P -L -@ and -e...
//		THE REST WORKS

// here we chdir and we update the vars cwd and oldpwd that are normally in env

static int	do_cd_update_env(char *arg, t_path_data *path_data)
{
	char	cwd[1024];

	if (chdir(arg) == -1)
		return (write_err("cd : No such file or directory\n"), 1);
	if (path_data->oldpwd != NULL)
		free(path_data->oldpwd);
	path_data->oldpwd = strdup(path_data->pwd);
	if (path_data->oldpwd == NULL)
		return (write_err("cd : Memory allocation failed for oldpwd\n"), 1);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (write_err("cd : getcwd failed\n"), 1);
	path_data->pwd = strdup(cwd);
	if (path_data->pwd == NULL)
		return (write_err("cd : Memory allocation failed for pwd\n")
			, free(path_data->oldpwd), 1);
	return (0);
}

// here we only handle cd -
// it does cd to the last directory in which we had cd

static int	only_dash(t_parsing_data *p_data, t_path_data *path_data, int i)
{
	if (ft_isequal(p_data->arg[i], "-"))
	{
		if (path_data->oldpwd == NULL)
			return (write_err("cd : OLDPWD not set\n"), 1);
		else if (p_data->arg[i + 1])
			return (write_err("cd : too many arguments\n"), 1);
		write(1, path_data->oldpwd, ft_strlen(path_data->oldpwd));
		write(1, "\n", 1);
		return (do_cd_update_env(path_data->oldpwd, path_data), 1);
	}
	return (0);
}

// here we handle -- and false arguments

static int	check_dash(t_parsing_data *p_data, t_path_data *path_data
		, char *home, int i)
{
	if (p_data->arg[i][0] == '-')
	{
		if (only_dash(p_data, path_data, i) == 1)
			return (1);
		else if (ft_isequal(p_data->arg[i], "--"))
		{
			if (!p_data->arg[i + 1])
			{
				if (!home)
					return (write_err(
							"cd : HOME environment variable not set\n"), 1);
				else
					do_cd_update_env(home, path_data);
			}
			else if (p_data->arg[i + 2])
				return (write_err("cd : too many argumets\n"), 1);
			else
				do_cd_update_env(p_data->arg[2], path_data);
		}
		else
			return (write_err("cd : Illegal option (--, -,[working])\n \
                     (-L, -P, -@, -e[not implemented])\n"), 1);
	}
	return (0);
}

// here we setup flags if we want to do them one day
// flags can be -P -PPPPPPPP {-PP -PPPPLLLLPLPLP}...
// LPe@

static int	setup_flags(t_parsing_data *p_data, t_path_data *path_data)
{
	int	i;

	i = 0;
	init_flags(path_data);
	while (p_data->arg[++i] && p_data->arg[i][1] && p_data->arg[i][0] == '-' && \
		does_contain_only(p_data->arg[i], "LPe@"))
		fill(p_data->arg[i], path_data);
	if (path_data->is_big_l || path_data->is_big_p
		|| path_data->is_e || path_data->is_at)
		return (write_err(" -L, -P, -e and -@ are not implemented yet :'(\n"), \
				-1);
	return (i);
}

//here is the main cd file that handle ~ and no args

int	cd(t_parsing_data *p_data, t_path_data *path_data)
{
	int	i;

	i = setup_flags(p_data, path_data);
	if (i == -1)
		return (1);
	if (p_data->arg[i])
	{
		if (check_dash(p_data, path_data, path_data->home, i))
			return (1);
		else if (p_data->arg[i + 1])
			return (write_err("cd : too many arguments\n"), 1);
		else if (ft_isequal(p_data->arg[i], "~"))
		{
			if (!path_data->home)
				return (write_err("cd : HOME environment variable not set\n"), \
				1);
			return (do_cd_update_env(path_data->home, path_data), 0);
		}
		return (do_cd_update_env(p_data->arg[i], path_data), 0);
	}
	if (!path_data->home)
		return (write_err("cd : HOME environment variable not set\n"), 1);
	return (do_cd_update_env(path_data->home, path_data), 0);
}
