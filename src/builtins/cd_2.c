/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lorey <loic.rey.vs@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 14:05:49 by lorey             #+#    #+#             */
/*   Updated: 2025/03/21 14:34:32 by lorey            ###   LAUSANNE.ch       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// here we chdir and we update the vars cwd and oldpwd that are normally in env

int	do_cd_update_env(char *arg, t_env_data *e_data)
{
	char	cwd[1024];

	if (chdir(arg))
		return (write_err("cd : No such file or directory\n"), -1);
	set_env(e_data, "OLDPWD", get_env(e_data, "PWD", NULL), true);
	if (get_env(e_data, "OLDPWD", NULL) == NULL)
		return (write_err("cd : Memory allocation failed for OLDPWD\n"), -1);
	getcwd(cwd, 1024);
	set_env(e_data, "PWD", cwd, true);
	if (get_env(e_data, "PWD", NULL) == NULL)
		return (write_err("cd : Memory allocation failed for PWD\n"), -1);
	return (0);
}
