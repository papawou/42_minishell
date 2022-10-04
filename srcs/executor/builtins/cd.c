/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvarrin <florian.varrin@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 16:32:54 by fvarrin           #+#    #+#             */
/*   Updated: 2022/09/28 18:28:41 by fvarrin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

unsigned int	execute_cd(t_list_el *env, t_command *command)
{
	int		status;
	char	*path;

	if (command->argv[1] != NULL && command->argv[2] != NULL)
	{
		print_custom_error("cd", NULL, "too many arguments");
		return (1);
	}
	if (command->argv[1])
	{
		status = chdir(command->argv[1]);
		if (status == -1)
			print_custom_error("cd", NULL, strerror(errno));
		return (status == -1);
	}
	path = get_env_value(env, "HOME");
	if (path == NULL)
	{
		print_custom_error("cd", NULL, "HOME not set");
		return (1);
	}
	status = chdir(path);
	if (status == -1)
		print_custom_error("cd", NULL, strerror(errno));
	free(path);
	return (status == -1);
}
