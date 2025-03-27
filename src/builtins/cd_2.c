/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:05:49 by lorey             #+#    #+#             */
/*   Updated: 2025/03/27 22:31:41 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

// here we chdir and we update the vars cwd and oldpwd that are normally in env

int	do_cd_update_env(char *arg, t_data *data)
{
	char	cwd[1024];
	char	*oldpwd;
	char	*pwd;

	if (chdir(arg))
		return (write_err("cd : No such file or directory\n"), -1);
	pwd = get_env(data->env, "PWD", NULL);
	set_env("OLDPWD", pwd, true, data);
	oldpwd = get_env(data->env, "OLDPWD", NULL);
	if (oldpwd == NULL)
		return (write_err("cd : Memory allocation failed for OLDPWD\n"), -1);
	getcwd(cwd, 1024);
	set_env("PWD", cwd, true, data);
	safe_free((void **)&pwd);
	pwd = get_env(data->env, "PWD", NULL);
	if (pwd == NULL)
		return (write_err("cd : Memory allocation failed for PWD\n"), -1);
	safe_free((void **)&pwd);
	safe_free((void **)&oldpwd);
	return (0);
}
