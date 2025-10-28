#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <sys/wait.h>

typedef struct s_env
{
	char	**vars;
	int		count;
	int		capacity;
}	t_env;

//builtins.c
int		builtin_echo(char **args);
int		builtin_cd(char **args);
int		builtin_pwd(char **args);
int		is_integer(char *str);
int		builtin_exit(char **args);
int		execute_builtins(char **args, t_env *env);

//builtins_env_variables.c
void	env_expand(t_env *env);
void	env_set(t_env *env, const char *var_str);
t_env	*env_create(void);
void	env_destroy(t_env *env);
void	env_unset(t_env *env, const char *name);
int		is_valid_var_name(char *str);
int		builtin_env(char **args, t_env *env);
int		builtin_export(char **args, t_env *env);
int		builtin_unset(char **args, t_env *env);

#endif