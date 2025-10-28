#include "minishell.h"

t_env	*env_create(void)
{
	t_env *env = malloc(sizeof(t_env));
	env->capacity = 10;
	env->count = 0;
	env->vars = malloc(sizeof(char *) * env->capacity);
	env->vars[0] = NULL;

	extern char	**environ;
	int	i;

	i = 0;
	while (environ[i] && i < (env->capacity - 1))
	{
		env_set(env, environ[i]);
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
	free(env);
}

void	env_expand(t_env *env)
{
	int	new_capacity;
	char	**new_vars;
	int	i;

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
	env->capacity =new_capacity;
}

void	env_set(t_env *env, const char *var_str)
{
	char	*equal_sign;
	char	*new_var;
	char	*name;
	int		i;

	equal_sign = ft_strchr(var_str, '=');
	if (!equal_sign)
		new_var = ft_strdup(var_str);
	else
	{
		new_var = ft_strdup(var_str);
		name = new_var;
		*equal_sign = '\0';
	}
	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->vars[i], name, ft_strlen(name)) == 0 && env->vars[i][ft_strlen(name)] == '=')
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

void	env_unset(t_env *env, const char *name)
{
	int	i;
	int	j;

	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->vars[i], name, ft_strlen(name)) == 0 && env->vars[i][ft_strlen(name)] == '=')
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

int	is_valid_var_name(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_env(char **args, t_env *env)
{
	int	i;
	(void)args;

	i = 0;
	while (i < env->count)
	{
		printf("%s\n", env->vars[i]);
		i++;
	}
	return (0);
}

int	builtin_export(char **args, t_env *env)
{
	if (!args[1])
		return (builtin_env(args, env));
	env_set(env, args[1]);
	return (0);
}

int	builtin_unset(char **args, t_env *env)
{
	if (!args[1])
		return (0);
	env_unset(env, args[1]);
	return (0);
}
