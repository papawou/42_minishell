/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvarrin <florian.varrin@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 12:55:18 by fvarrin           #+#    #+#             */
/*   Updated: 2022/09/18 15:36:24 by fvarrin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define FILE_PERMISSION_IF_CREATED 0664

# include "libft.h"

extern char	**environ;

typedef enum e_error_codes {
	ERR_ALLOCATING_MEMORY = 1,
	ERR_OPENING_FILE = 2,
	ERR_FORKING_PROCESS = 3,
}	t_error_codes;

typedef enum e_token_type {
	WORD_W_ENV_EXP,
	WORD_WO_ENV_EXP,
	O_SIMPLE_OP,
	O_APPEND_OP,
	I_SIMPLE_OP,
	I_HEREDOC_OP,
	SPACE_DELIMITER,
	PIPE
}	t_token_type;

typedef struct s_command {
	char		*in;
	char		*out;
	char		*bin;
	char		**argv;
	t_list_el	*tokens;
	int			return_value;
	_Bool		out_in_append_mode;
}	t_command;

typedef struct s_token {
	char			*value;
	t_token_type	type;
}	t_token;

typedef struct s_execution_plan {
	t_command	**commands;
	int			number_of_commands;
}	t_execution_plan;

typedef struct s_env_variable {
	char	*token_value;
	int		token_value_length;
	char	*key;
	int		key_length;
	char	*value;
	int		value_length;
}	t_env_variable;

/** Utils **/
char				*get_env_value(char *env);

char				*trim_space(char *source);
int					open_file(char *path, int flags);
char				*create_base_str(void);

/** Prompter **/
void				print_welcome_message(void);
char				*prompt(char *line_read);

/* Signals */
void				configure_termios(void);
void				set_parent_signals(void);
void				set_child_signals(void);
void				unset_parent_signals(void);

/** Parser **/
t_execution_plan	*parse_line(char *line);

/* Builder */
t_execution_plan	*init_execution_plan(int number_of_commands);
void				destroy_execution_plan(t_execution_plan *execution_plan);

t_token				*init_token(void);
t_command			*init_command(void);

/* Tokenizer */
t_list_el			*tokenize_line(char *line);

_Bool				check_quote_closed(char *str);
void				set_simple_word(char **str, t_token *token);
void				set_double_quote_word(char **str, t_token *token);
void				set_single_quote_word(char **str, t_token *token);
void				set_space_delimiter(char **str, t_token *token);
_Bool				is_a_simple_output(char *str);
_Bool				is_a_append_output(char *str);
_Bool				is_a_simple_input(char *str);
_Bool				is_a_heredoc_input(char *str);
_Bool				is_a_pipe(char *str);
_Bool				is_an_operator(char *str);
_Bool				is_operator_symbol(char c);
void				set_operator(char **str, t_token *token);

_Bool				has_more_tokens(char *str);

/* Parser */
t_execution_plan	*parse_tokens(t_list_el *tokens);

_Bool				verify_tokens(t_list_el *tokens);

void				parse_env_variables(t_list_el *tokens);
_Bool				str_has_env_variable(char *str);
char				*extract_env_variable_key_from_str(char *str);
int					calculate_env_variable_expanded_length(
						t_env_variable *env_variable);

int					count_number_of_commands(t_list_el *tokens);
void				set_io_from_tokens(t_command *command);
void				set_argv_from_tokens(t_command *command, char **str);

/** Executor **/
int					execute_plan(t_execution_plan *execution_plan);
void				execute_command(t_execution_plan *execution_plan,
						int **pipes, int index);

int					**create_pipes(int number_of_child_processes, int **pipes);
void				close_pipes_in_child_process(int **pipes,
						int number_of_child_processes, int index);
void				close_pipes_in_main_process(
						int **pipes, int number_of_child_processes);
void				destroy_pipes(int number_of_child_processes, int **pipes);

int					*create_processes(t_execution_plan *execution_plan,
						int *pids, int **pipes);
int					count_total_process(int number_of_child_processes);

void				route_command_io(
						t_command *command,
						int **pipes,
						int index,
						int number_of_commands
						);

_Bool				is_a_builtins(char *bin);
void				execute_builtins(t_command *command);

void				execute_echo(t_command *command);

#endif