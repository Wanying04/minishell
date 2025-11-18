#include "minishell.h"

char **env_to_array(t_env *env)
{
	char	**env_array;
	int		i;

	if (!env || !env->vars)
		return (NULL);
	env_array = malloc(sizeof(char *) * (env->count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (i < env->count)
	{
		env_array[i] = ft_strdup(env->vars[i]);
		if (!env_array[i])
		{
			while (--i >= 0)
				free(env_array[i]);
			free(env_array);
			return (NULL);
		}
		i++;
	}
	env_array[env->count] = NULL;
	return (env_array);
}

char	*search_in_directory(char *dir, char *cmd)
{
	char	full_path[1024];

	ft_strlcpy(full_path, dir, sizeof(full_path));
	ft_strlcat(full_path, "/", sizeof(full_path));
	ft_strlcat(full_path, cmd, sizeof(full_path));
	if (access(full_path, X_OK) == 0)
		return (ft_strdup(full_path));
	return (NULL);
}

char	*search_in_path(char *cmd, char *path_env)
{
	char	*path_copy;
	char	*current;
	char	*end;
	char	*result;
	char	saved_char;

	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
	current = path_copy;
	while (*current)
	{
		end = current;
		while (*end && *end != ':')
			end++;
		saved_char = *end;
		*end = '\0';
		result = search_in_directory(current, cmd);
		*end = saved_char;
		if (result)
		{
			free(path_copy);
			return (result);
		}
		if (*end == ':')
			current = end + 1;
		else
			current = end;
	}
	free(path_copy);
	return (NULL);
}

char	*find_command_path(char *cmd, t_env *env)
{
	char	*path_env;
	char	*result;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	path_env = env_get(env, "PATH");
	if (!path_env || path_env[0] == '\0')
		return (NULL);
	result = search_in_path(cmd, path_env);
	return (result);
}
