/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvarrin <florian.varrin@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/25 16:32:54 by fvarrin           #+#    #+#             */
/*   Updated: 2022/10/08 16:23:36 by fvarrin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int	print_cd_error(char *message, int error_number)
{
	print_custom_error("cd", NULL, message);
	return (error_number);
}

unsigned int	execute_cd(t_list_el *env, t_command *command)
{
	int		status;
	char	*path;

	if (command->argv[1] != NULL && command->argv[2] != NULL)
		return (print_cd_error("too many arguments", 1));
	if (command->argv[1])
	{
		status = chdir(command->argv[1]);
		if (status == -1)
			return (print_cd_error(strerror(errno), 1));
	}
	path = get_env_value(env, "HOME");
	if (path == NULL)
		return (print_cd_error("HOME not set", 1));
	status = chdir(path);
	if (status == -1)
		print_cd_error(strerror(errno), 1);
	free(path);
	return (1);
}
