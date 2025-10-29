#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <sys/wait.h>

// Forward declarations
typedef struct s_command t_command;
typedef struct s_env t_env;

// ============ Executor 主接口函数 ============
int		execute(t_command *cmd, t_env *env);
int		execute_pipeline(t_command *cmd, t_env *env);
int		handle_redirections(t_command *cmd);
int		is_builtin_command(char **argv);

//builtins.c
int		builtin_echo(char **args);
int		builtin_cd(char **args, t_env *env);
int		builtin_pwd(char **args);
int		is_integer(char *str);
int		builtin_exit(char **args);
int		execute_builtins(char **args, t_env *env);

//builtins_env_variables.c
int		builtin_env(char **args, t_env *env);
int		builtin_export(char **args, t_env *env);
int		builtin_unset(char **args, t_env *env);

#endif