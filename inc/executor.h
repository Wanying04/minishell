#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <sys/wait.h>

// Forward declarations
typedef struct s_command t_command;
typedef struct s_env t_env;

/*
 * executor.h - Executor module function declarations
 * 
 * Note: Public interfaces (execute, env_create, etc.) are declared in minishell.h
 * This file contains all functions used internally by the executor module
 */

// ============ Execution core ============
int		execute_pipeline(t_command *cmd, t_env *env);
int		execute_pipeline_core(t_command *cmd, t_env *env);
int		execute_single_command(t_command *cmd, t_env *env);
int		is_builtin_command(t_command *cmd);
int		execute_builtins(t_command *cmd, t_env *env);

// ============ Redirections and external commands ============
int		handle_redirections(t_command *cmd, t_env *env);
int		handle_redirections_only(t_command *cmd);
void	child_process(t_command *cmd, t_env *env);
int		execute_external_command(t_command *cmd, t_env *env);

// ============ Builtins - Built-in commands ============
int		builtin_echo(t_command *cmd);
int		builtin_pwd(t_command *cmd);
int		builtin_exit(t_command *cmd);
int		builtin_env(t_command *cmd, t_env *env);
int		builtin_export(t_command *cmd, t_env *env);
int		builtin_unset(t_command *cmd, t_env *env);
int		builtin_cd(t_command *cmd, t_env *env);

// ============ Builtin helpers ============
int		is_integer(char *str);
int		is_valid_var_name(char *str);
void	update_pwd_oldpwd(t_env *env, char *old_cwd, char *new_cwd);

// ============ Environment management internal functions ============
void	env_expand(t_env *env);

// ============ Path resolution and env conversion ============
char	**env_to_array(t_env *env);
char	*find_command_path(char *cmd, t_env *env);

void	reset_signals_to_default(void);

#endif