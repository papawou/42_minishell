/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmendes <kmendes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 13:55:46 by fvarrin           #+#    #+#             */
/*   Updated: 2022/10/08 16:10:29 by fvarrin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <unistd.h>
#include <stdlib.h>

/**
 * Utility to get the total number of process from the number of child process.
 * The goal is to avoid the "1" magic number at several places
 *
 * @param {int} number_of_child_processes
 *
 * @return {int} total_number_of_processes
 */
int	count_total_process(int number_of_child_processes)
{
	return (number_of_child_processes + 1);
}

void	handle_child_process(
			t_execution_plan *execution_plan,
			int **pipes,
			int index
		)
{
	int		number_of_child_processes;
	int		cmd_ret;

	number_of_child_processes = execution_plan->number_of_commands;
	set_child_signals();
	close_pipes_in_child_process(pipes, number_of_child_processes, index);
	cmd_ret = execute_command(execution_plan, pipes, index);
	destroy_pipes(number_of_child_processes, pipes);
	destroy_execution_plan(execution_plan);
	ft_lstclear(execution_plan->env, &destroy_environ_el);
	exit(cmd_ret);
}

/**
 * Allocate memory for the pids and fork for every processes
 *
 * @param {t_execution_plan *} execution_plan
 * @param {int *} pids
 * @param {int **} pipes
 *
 * @return {int *} return a pointer to the pids
 */
int	create_processes(t_execution_plan *execution_plan, int **pipes)
{
	int		i;
	int		number_of_child_processes;
	int		last_pid;

	number_of_child_processes = execution_plan->number_of_commands;
	i = 0;
	while (i < number_of_child_processes)
	{
		if (execution_plan->commands[i]->heredoc != NULL)
			execute_heredocs(execution_plan->commands[i]);
		last_pid = fork();
		if (last_pid == -1)
			break ;
		if (last_pid == 0)
			handle_child_process(execution_plan, pipes, i);
		i++;
	}
	return (last_pid);
}

int	create_shellscript(t_execution_plan *execution_plan, int **pipes)
{
	int		i;
	int		number_of_child_processes;
	int		cmd_ret;

	number_of_child_processes = execution_plan->number_of_commands;
	i = 0;
	while (i < number_of_child_processes)
	{
		if (execution_plan->commands[i]->heredoc != NULL)
			execute_heredocs(execution_plan->commands[i]);
		cmd_ret = execute_command(execution_plan, pipes, i);
		i++;
	}
	return (cmd_ret);
}
