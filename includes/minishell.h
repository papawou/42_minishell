/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmendes <kmendes@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/29 12:55:18 by fvarrin           #+#    #+#             */
/*   Updated: 2022/10/11 01:06:01 by kmendes          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define FILE_PERMISSION_IF_CREATED 0664
# define SHELL_NAME "minishell"
# define FOREVER 42

# include <signal.h>
# include <stdio.h>
# include <unistd.h>
# include "libft.h"

extern char						**environ;
extern volatile sig_atomic_t	g_env_exit;

# define TMP_FILE "/tmp/.minishell.heredoc"

typedef enum e_error_codes {
	ERR_ALLOCATING_MEMORY = 1,
	ERR_OPENING_FILE = 2,
	ERR_FORKING_PROCESS = 3,
	ERR_PIPING = 5,
	ERR_EXEC = 6
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

typedef struct s_file_redirect {
	char	*file;
	int		file_fd;
	int		std_copy;
	int		mode;
}	t_file_redirect;

typedef struct s_command {
	t_list_el		*in;
	t_list_el		*out;
	char			*bin;
	char			**argv;
	t_list_el		*heredoc;
	t_list_el		*tokens;
	int				return_value;
	int				idx;
}	t_command;

typedef struct s_token {
	char			*value;
	t_token_type	type;
}	t_token;

typedef struct s_execution_plan {
	t_command	**commands;
	int			number_of_commands;
	t_list_el	**env;
	_Bool		need_to_fork;
}	t_execution_plan;

typedef struct s_environ_el {
	char	*key;
	char	*value;
}	t_environ_el;

typedef struct s_env_variable {
	char	*token_value;
	int		token_value_length;
	char	*start_of_variable;
	char	*key;
	int		key_length;
	char	*value;
	int		value_length;
}	t_env_variable;

/** Utils **/
char				*trim_space(char *source);
int					open_file(char *path, int flags);
char				*create_empty_str(void);

char				*get_current_dir(void);

void				print_custom_error(
						char *prefix,
						char *attribute,
						char *message
						);
_Bool				unexpected_token(char *value);

/** Prompter **/
void				print_welcome_message(void);
char				*prompt(char *line_read);

/** Signals **/
void				configure_termios(void);
void				set_parent_signals(void);
void				set_child_signals(void);
void				unset_parent_signals(void);

/** Environ **/
void				destroy_environ_el(void *el);
void				add_environ_el(t_list_el **entry, char *key_value);
void				remove_environ_el(t_list_el **entry, char *key);
t_environ_el		*init_environ_el(char *key_value);
t_list_el			*parse_environ(void);

char				**environ_el_to_char_2d(t_list_el *entry);
void				free_environ_char_2d(char **src);
void				print_environ_char_2d(char **char_2d);

_Bool				is_valid_key_value_env(char *key_value);
_Bool				extract_key_value(char *key_value,
						char **key, char **value);

t_environ_el		*get_environ_el(t_list_el *entry, char *key);
char				*get_env_value(t_list_el *env, char *key);

/** Parser **/
t_execution_plan	*parse_line(t_list_el *env, char *line);

/* Builder */
t_execution_plan	*init_execution_plan(int number_of_commands);
void				destroy_execution_plan(t_execution_plan *execution_plan);

t_token				*init_token(void);
t_command			*init_command(void);
void				destroy_command(t_command *command);

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
_Bool				is_an_operator(char *str);
_Bool				is_operator_symbol(char c);
void				set_operator(char **str, t_token *token);

void				eat_operator(
						char **str,
						char operator,
						int number_of_operator
						);

_Bool				is_a_pipe(char *str);
void				set_pipe(char **str, t_token *token);

_Bool				has_more_tokens(char *str);
_Bool				is_quote(char c);

void				destroy_token(void *token);

/* Parser */
t_execution_plan	*parse_all_tokens(t_list_el *tokens);

_Bool				verify_tokens(t_list_el *tokens);
_Bool				verify_first_is_not_pipe(t_list_el *tokens);

t_env_variable		**init_env_variables(t_list_el *env, char *token_value);
void				parse_env_variables(t_list_el *env, t_list_el *tokens);
t_env_variable		*destroy_env_variable(t_env_variable *env_variable);
int					count_number_of_env_variable(const char *str);
_Bool				string_has_env_variable(const char *str);
char				*expand_env_variables_string(t_list_el *env, char *value);
char				*extract_env_variable_key_from_str(
						t_env_variable *env_variable,
						char **cursor
						);
int					calculate_env_variable_expanded_length(
						t_env_variable *env_variable);

int					count_number_of_commands(t_list_el *tokens);
void				set_io_from_tokens(t_command *command);
void				set_argv_from_tokens(t_command *command, char **str);

_Bool				is_io_token(t_token *token);
_Bool				has_heredoc_token(t_list_el *tokens);

_Bool				check_if_need_to_fork(t_execution_plan *execution_plan);

/** Executor **/
int					execute_plan(t_execution_plan *execution_plan);
int					execute_command(t_execution_plan *execution_plan,
						int **pipes, int index);

int					**create_pipes(int number_of_child_processes);
void				close_pipes_in_child_process(int **pipes,
						int number_of_child_processes, int index);
void				close_pipes_in_main_process(
						int **pipes, int number_of_child_processes);
void				destroy_pipes(int number_of_child_processes, int **pipes);

int					create_processes(t_execution_plan *execution_plan,
						int **pipes);
int					execute_single_without_fork(
						t_execution_plan *execution_plan, int **pipes);
int					count_total_process(int number_of_child_processes);

_Bool				route_command_io(
						t_command *command,
						int **pipes,
						int index,
						int number_of_commands
						);
void				route_back_command_io(t_command *command);

t_file_redirect		*init_file_redirect(char *file);
void				destroy_file_redirect(void *file_redirect_arg);

_Bool				open_tmp_file(int *tmp_file_fd, int idx_cmd);
int					execute_heredocs(t_list_el *env, t_command *command);
void				execute_heredoc(
						t_list_el *env,
						char *delimiter,
						int tmp_file_fd
						);
void				set_heredoc_signals(void);

char				*get_program_path(t_list_el *env, t_command *command);

_Bool				is_a_builtins(char *bin);
int					execute_builtins(t_list_el **env, t_command *command);

int					clear_if_exec_fail(
						t_execution_plan *execution_plan,
						int **pipes,
						char *program_path,
						char **environ_as_arr
						);
int					handle_builtins_execution(
						t_execution_plan *execution_plan,
						t_command *command
						);
char				*find_program_path(
						t_execution_plan *execution_plan,
						t_command *command
						);

unsigned int		execute_echo(t_command *command);
unsigned int		execute_cd(t_list_el *env, t_command *command);
unsigned int		execute_pwd(void);
unsigned int		execute_export(t_list_el **env, t_command *command);
unsigned int		execute_unset(t_list_el **env, t_command *command);
unsigned int		execute_env(t_list_el **env);
unsigned int		execute_exit(t_command *command);

int					execve_process_error(char *command, int execve_errno);
#endif