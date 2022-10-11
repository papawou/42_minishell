/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvarrin <florian.varrin@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 17:38:35 by fvarrin           #+#    #+#             */
/*   Updated: 2022/10/10 18:40:56 by fvarrin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdbool.h>

/**
 * Define if a string is an operator
 *
 * @param {char *} str
 * `
 * @return {_Bool} is_an_operator
 */
_Bool	is_an_operator(char *str)
{
	if (is_a_append_output(str))
		return (true);
	if (is_a_simple_output(str))
		return (true);
	if (is_a_heredoc_input(str))
		return (true);
	if (is_a_simple_input(str))
		return (true);
	return (false);
}

/**
 * Return true if is an operator symbol (`<`, `>` or `|`)
 * @param {char} c
 *
 * @return {_Bool}
 */
_Bool	is_operator_symbol(char c)
{
	if (c == '<' || c == '>')
		return (true);
	return (false);
}

/**
 * Move the string until is neither an operator symbol nor a space
 * and re-assign the string pointer to the new position
 *
 * @param {char **} str
 */
void	eat_operator(char **str, char operator, int number_of_operator)
{
	int		eaten_operator;

	eaten_operator = 0;
	while ((**str == operator || **str == ' ')
		&& eaten_operator < number_of_operator)
	{
		if (**str == operator)
			eaten_operator++;
		*str = &((*str)[1]);
	}
}

/**
 * Set the correct operator type to a given token
 *
 * @param {char **} str
 * @param {t_token *} token
 */
void	set_operator(char **str, t_token *token)
{
	if (is_a_append_output(*str) == true)
	{
		token->type = O_APPEND_OP;
		token->value = ft_strdup(">>");
		eat_operator(str, '>', 2);
	}
	else if (is_a_simple_output(*str) == true)
	{
		token->type = O_SIMPLE_OP;
		token->value = ft_strdup(">");
		eat_operator(str, '>', 1);
	}
	else if (is_a_heredoc_input(*str) == true)
	{
		token->type = I_HEREDOC_OP;
		token->value = ft_strdup("<<");
		eat_operator(str, '<', 2);
	}
	else if (is_a_simple_input(*str) == true)
	{
		token->type = I_SIMPLE_OP;
		token->value = ft_strdup("<");
		eat_operator(str, '<', 1);
	}
}
