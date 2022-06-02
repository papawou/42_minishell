/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvarrin <florian.varrin@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 17:02:35 by fvarrin           #+#    #+#             */
/*   Updated: 2022/05/30 18:00:44 by fvarrin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

char	**get_io_from_begining(char **arguments, t_command *command)
{
	int		i;
	int		args_begin;

	if (ft_strcmp(arguments[0], "<") != 0)
		return (arguments);
	command->in = ft_strdup(arguments[1]);
	i = 2;
	args_begin = i;
	while (--i >= 0)
	{
		free(arguments[i]);
		arguments[i] = NULL;
	}
	return (&arguments[args_begin]);
}

char	**get_io_from_end(char **arguments, t_command *command)
{
	int		i;

	i = 0;
	while (arguments[i] && ft_strcmp(arguments[i], "<") != 0
		&& ft_strcmp(arguments[i], ">") != 0)
		i++;
	if (arguments[i] == NULL)
		return (arguments);
	if (ft_strcmp(arguments[i], ">") == 0)
		command->out = ft_strdup(arguments[i + 1]);
	else
		command->in = ft_strdup(arguments[i + 1]);
	while (arguments[i])
	{
		if (ft_strcmp(arguments[i], ">") == 0)
			command->out = ft_strdup(arguments[i + 1]);
		free(arguments[i]);
		arguments[i] = NULL;
		i++;
	}
	return (arguments);
}

char	**get_io_for_command(char **arguments, t_command *command)
{
	arguments = get_io_from_begining(arguments, command);
	arguments = get_io_from_end(arguments, command);
	return (arguments);
}
