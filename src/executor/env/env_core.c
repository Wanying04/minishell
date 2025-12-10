#include "minishell.h"

t_env	*env_create(char **envp)
{
	t_env	*env;
	int		i;

	env = ft_calloc(sizeof(t_env), 1);
	env->capacity = 100;
	env->count = 0;
	env->exit_status = SUCCESS;
	env->vars = ft_calloc(sizeof(char *), env->capacity);
	env->vars[0] = NULL;
	env->pwd = ft_calloc(sizeof(t_pwd), 1);
	env->pwd->pwd = NULL;
	env->pwd->oldpwd = NULL;
	if (!envp)
		return (env);
	i = 0;
	while (envp[i])
	{
		env_set(env, envp[i]);
		i++;
	}
	return (env);
}

void	env_destroy(t_env *env)
{
	int	i;

	i = 0;
	while (i < env->count)
	{
		free(env->vars[i]);
		i++;
	}
	free(env->vars);
	if (env->pwd)
	{
		if (env->pwd->pwd)
			free(env->pwd->pwd);
		if (env->pwd->oldpwd)
			free(env->pwd->oldpwd);
		free(env->pwd);
	}
	free(env);
}

void	env_expand(t_env *env)
{
	int		new_capacity;
	char	**new_vars;
	int		i;

	new_capacity = env->capacity * 2;
	new_vars = ft_calloc(sizeof(char *), new_capacity);
	i = 0;
	while (i < env->count)
	{
		new_vars[i] = env->vars[i];
		i++;
	}
	free(env->vars);
	env->vars = new_vars;
	env->capacity = new_capacity;
}
