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
	while (environ[i])
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
	int	i;

	if (!args[1])
		return (builtin_env(args, env));
	i = 1;
	while (args[i])
	{
		if (!is_valid_var_name(args[i]))
		{
			printf("export: not a valid identifier\n");
			return (1);
		}
		env_set(env, args[i]);
		i++;
	}
	return (0);
}

int	builtin_unset(char **args, t_env *env)
{
	int	i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		if (!is_valid_var_name(args[i]))
		{
			printf("unset: not a valid identifier\n");
			return (1);
		}
		env_unset(env, args[i]);
		i++;
	}
	return (0);
}
