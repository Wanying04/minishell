#include "minishell.h"

void	env_expand(t_env *env)
{
	int		new_capacity;
	char	**new_vars;
	int		i;

	new_capacity = env->capacity * 2;
	new_vars = malloc(sizeof(char *) * new_capacity);
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

void	env_set(t_env *env, const char *var_str)
{
	char	*new_var;
	char	*equal_sign;
	int		name_len;
	int		i;

	new_var = ft_strdup(var_str);
	equal_sign = ft_strchr(new_var, '=');
	if (equal_sign)
		name_len = equal_sign - new_var;
	else
		name_len = ft_strlen(new_var);
	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->vars[i], new_var, name_len) == 0 && 
			(env->vars[i][name_len] == '=' || env->vars[i][name_len] == '\0'))
		{
			free(env->vars[i]);
			env->vars[i] = new_var;
			return ;
		}
		i++;
	}
	if (env->count >= env->capacity)
		env_expand(env);
	env->vars[env->count] = new_var;
	env->count++;
	env->vars[env->count] = NULL;
}

t_env	*env_create(char **envp)
{
	t_env	*env;
	int		i;

	env = malloc(sizeof(t_env));
	env->capacity = 100;
	env->count = 0;
	env->exit_status = 0;
	env->vars = malloc(sizeof(char *) * env->capacity);
	env->vars[0] = NULL;
	env->pwd = malloc(sizeof(t_pwd));
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

void	env_unset(t_env *env, const char *name)
{
	int	i;
	int	j;
	int	name_len;

	name_len = ft_strlen(name);
	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->vars[i], name, name_len) == 0 && (env->vars[i][name_len] == '=' || env->vars[i][name_len] == '\0'))
		{
			free(env->vars[i]);
			j = i;
			while (j < env->count - 1)
			{
				env->vars[j] = env->vars[j + 1];
				j++;
			}
			env->count--;
			env->vars[env->count] = NULL;
			return ;
		}
		i++;
	}
}

char	*env_get(t_env *env, const char *name)
{
	int		i;
	int		name_len;
	char	*equal_sign;

	if (!env || !name)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->vars[i], name, name_len) == 0 
			&& env->vars[i][name_len] == '=')
		{
			equal_sign = ft_strchr(env->vars[i], '=');
			if (equal_sign)
				return (equal_sign + 1);
		}
		i++;
	}
	return (NULL);
}
