/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env-vars-utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvarrin <florian.varrin@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 15:01:56 by fvarrin           #+#    #+#             */
/*   Updated: 2022/09/18 15:05:03 by fvarrin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdbool.h>
#include <stdlib.h>

_Bool	str_has_env_variable(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i++] == '$')
			return (true);
	}
	return (false);
}

char	*extract_env_variable_key_from_str(char *str)
{
	int		i;
	int		length;
	char	*start_pos;

	i = 0;
	length = 0;
	start_pos = NULL;
	while (str[i])
	{
		if (length > 0 && !ft_isalnum(str[i]))
			break ;
		else if (str[i] == '$')
			start_pos = &(str[i + 1]);
		else if (start_pos != NULL)
			length++;
		i++;
	}
	return (ft_strndup(start_pos, length));
}

int	calculate_env_variable_expanded_length(t_env_variable *env_variable)
{
	return (
		env_variable->token_value_length
		- env_variable->key_length
		+ env_variable->value_length
	);
}
