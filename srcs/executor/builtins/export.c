/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvarrin <florian.varrin@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 16:12:26 by fvarrin           #+#    #+#             */
/*   Updated: 2022/10/08 16:24:55 by fvarrin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdbool.h>
#include <stdlib.h>

unsigned int	execute_export(t_list_el **env, t_command *command)
{
	int		i;
	_Bool	has_an_error;

	has_an_error = false;
	i = 1;
	while (command->argv[i])
	{
		if (!is_valid_key_value_env(command->argv[i]))
		{
			has_an_error = true;
			print_custom_error("export",
				command->argv[i], "not a valid identifier");
		}
		else
			add_environ_el(env, command->argv[i]);
		++i;
	}
	if (has_an_error)
		return (1);
	return (0);
}
