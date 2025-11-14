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

// ============ Execute.c - Execution functions ============
int		execute_pipeline(t_command *cmd, t_env *env);
int		is_builtin_command(t_command *cmd);

// ============ Redirections.c - Redirections and external commands ============
int		redirections(void);
int		handle_redirections(t_command *cmd);
int		execute_external_command(t_command *cmd, t_env *env);

// ============ Builtins.c - Built-in commands ============
int		builtin_echo(t_command *cmd);
int		builtin_pwd(t_command *cmd);
int		builtin_exit(t_command *cmd);
int		is_integer(char *str);
int		execute_builtins(t_command *cmd, t_env *env);

// ============ Builtins_env_variables.c - Environment variable built-ins ============
int		is_valid_var_name(char *str);
int		builtin_env(t_command *cmd, t_env *env);
int		builtin_export(t_command *cmd, t_env *env);
int		builtin_unset(t_command *cmd, t_env *env);

// ============ Cd.c - CD command ============
void	update_pwd_oldpwd(t_env *env, char *old_cwd, char *new_cwd);
char	*resolve_cd_path(char *arg, t_env *env, int *should_free);
char	*get_cd_path(char *arg, t_env *env, int *err, int *should_free);
int		builtin_cd_execute(char *arg, t_env *env);
int		builtin_cd(t_command *cmd, t_env *env);

// ============ Env.c - Environment management internal functions ============
void	env_expand(t_env *env);

#endif