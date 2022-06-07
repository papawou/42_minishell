/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmendes <kmendes@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 12:36:29 by fvarrin           #+#    #+#             */
/*   Updated: 2022/06/07 14:27:39 by kmendes          ###   ########.fr       */
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

int	main(int argc, __attribute__((unused)) char **argv)
{
	char				*line_read;
	t_execution_plan	*execution_plan;


	if (argc != 1)
	{
		print_usage();
		return (-3);
	}

	configure_termios();
	set_parent_signals();

	line_read = NULL;
	print_welcome_message();
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
