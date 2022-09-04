/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvarrin <florian.varrin@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/09 15:23:57 by fvarrin           #+#    #+#             */
/*   Updated: 2022/09/04 15:19:04 by fvarrin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

 /**
  *
  * Return the next token if exist
  *
  * @param {char *} str
  *
  * @return {t_token *} token
  */
t_token	*get_next_token(char **str)
{
	t_token		*token;

	if (!has_more_tokens(*str))
		return (NULL);
	token = init_token();
	if (is_an_operator(*str))
		set_operator(str, token);
	else if (**str == ' ')
		set_space_delimiter(str, token);
	else if (**str == '"')
		set_double_quote_word(str, token);
	else if (**str == '\'')
		set_single_quote_word(str, token);
	else
		set_simple_word(str, token);
	return (token);
}

/**
 * Add a space token to the front of the list
 *
 * @param {t_list_el **} list
 */
void	add_space_token(t_list_el **list)
{
	t_token		*token;

	token = init_token();
	token->type = SPACE_DELIMITER;
	ft_lstadd_front(list, ft_lstnew(token));
}

/**
 *
 * Get tokens for a line
 *
 * @param {char *} line
 *
 * @return {t_list_el *}
 */
t_list_el	*tokenize_line(char *line)
{
	char		**cursor;
	t_list_el	*list;
	t_token		*token;

	list = NULL;
	cursor = &line;
	token = get_next_token(cursor);
	if (token != NULL)
		ft_lstadd_back(&list, ft_lstnew(token));
	while (token != NULL)
	{
		token = get_next_token(cursor);
		if (token)
			ft_lstadd_back(&list, ft_lstnew(token));
	}
	if (((t_token *)list->content)->type != SPACE_DELIMITER)
		add_space_token(&list);
	return (list);
}
