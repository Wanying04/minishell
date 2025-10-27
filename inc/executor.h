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
int		buildin_echo(char **args);
int		buildin_cd(char **args);
int		buildin_pwd(char **args);
int		buildin_exit(char **args);
int		execute_builtins(char **args, t_env *env);

//builtins_env_variables.c
t_env	*env_create(void);
void	env_destroy(t_env *env);
void	env_expand(t_env *env);
void	env_set(t_env *env, const char *var_str);
void	env_unset(t_env *env, const char *name);
int		buildin_env(char **args, t_env *env);
int		buildin_export(char **args, t_env *env);
int		buildin_unset(char **args, t_env *env);

#endif