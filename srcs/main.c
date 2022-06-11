/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmendes <kmendes@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 12:36:29 by fvarrin           #+#    #+#             */
/*   Updated: 2022/06/11 14:38:39 by fvarrin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <signal.h>

#include <termios.h>

#include "libft.h"
#include "minishell.h"

void	print_usage(void)
{
	printf("%s only work in interactive mode without any arguments\n", BIN_NAME);
}

void	run_prompt(void)
{
	t_execution_plan	*execution_plan;
	char				*line_read;

	line_read = NULL;
	while (42)
	{
		line_read = prompt(line_read);
		line_read = trim_space(line_read);
		if (line_read == NULL)
			break ;
		if (*line_read)
		{
			unset_parent_signals();
			execution_plan = parse_line(line_read);
			execute_plan(execution_plan);
			destroy_execution_plan(execution_plan);
			set_parent_signals();
		}
	}
}

int	main(int argc, __attribute__((unused)) char **argv)
{

	if (argc != 1)
	{
		print_usage();
		return (-3);
	}
	configure_termios();
	set_parent_signals();
	print_welcome_message();
	run_prompt();
}
