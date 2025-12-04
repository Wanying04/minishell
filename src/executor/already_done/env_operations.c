#include "minishell.h"

static int	update_if_exists(t_env *env, char *new_var, int name_len)
{
	int	i;

	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->vars[i], new_var, name_len) == 0
			&& (env->vars[i][name_len] == '='
				|| env->vars[i][name_len] == '\0'))
		{
			free(env->vars[i]);
			env->vars[i] = new_var;
			return (SUCCESS);
		}
		i++;
	}
	return (FAILURE);
}

void	env_set(t_env *env, const char *var_str)
{
	char	*new_var;
	char	*equal_sign;
	int		name_len;

	new_var = ft_strdup(var_str);
	equal_sign = ft_strchr(new_var, '=');
	if (equal_sign)
		name_len = equal_sign - new_var;
	else
		name_len = ft_strlen(new_var);
	if (update_if_exists(env, new_var, name_len) == SUCCESS)
		return ;
	if (env->count >= env->capacity)
		env_expand(env);
	env->vars[env->count] = new_var;
	env->count++;
	env->vars[env->count] = NULL;
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

void	env_unset(t_env *env, const char *name)
{
	int	i;
	int	j;
	int	name_len;

	name_len = ft_strlen(name);
	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->vars[i], name, name_len) == 0 && 
			env->vars[i][name_len] == '=')
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
