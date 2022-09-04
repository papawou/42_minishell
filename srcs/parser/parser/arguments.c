/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvarrin <florian.varrin@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 19:20:48 by fvarrin           #+#    #+#             */
/*   Updated: 2022/09/04 14:48:43 by fvarrin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stdio.h>

#include "minishell.h"

#include <stdlib.h>

void	skip_n_elements(
		t_list_el **current_el_ptr,
		t_list_el **last_el_ptr,
		int n
		)
{
	int			i;
	t_list_el	*current_el;
	t_list_el	*last_el;

	last_el = *last_el_ptr;
	current_el = *current_el_ptr;
	i = 0;
	while (i < n)
	{
		last_el = current_el;
		current_el = current_el->next;
		i++;
	}
	*current_el_ptr = current_el;
	*last_el_ptr = last_el;
}

void	handle_input_simple_for_argv(
		t_list_el **current_el,
		t_list_el **last_el
		)
{
	if (*last_el == NULL)
		skip_n_elements(current_el, last_el, 2);
	else
		skip_n_elements(current_el, last_el, 1);
}

void	set_next_argv_str(t_command *command, char **str, int *i)
{
	command->argv[(*i)++] = *str;
	*str = create_base_str();
}

void	set_argv_from_tokens(t_command *command, char **str)
{
	int			i;
	t_list_el	*current_el;
	t_list_el	*last_el;
	t_token		*token;

	i = 0;
	current_el = command->tokens;
	last_el = NULL;
	while (current_el)
	{
		token = (t_token *)current_el->content;
		if (token->type == I_SIMPLE_OP)
			handle_input_simple_for_argv(&current_el, &last_el);
		if (token->type == O_SIMPLE_OP || token->type == O_APPEND_OP)
			break ;
		if (token->type == WORD_W_ENV_EXP || token->type == WORD_WO_ENV_EXP)
			*str = ft_strjoin(*str, token->value);
		else if (token->type == SPACE_DELIMITER)
			set_next_argv_str(command, str, &i);
		skip_n_elements(&current_el, &last_el, 1);
	}
	if ((*str)[0] != '\0')
		command->argv[i++] = *str;
	command->argv[i] = NULL;
}
