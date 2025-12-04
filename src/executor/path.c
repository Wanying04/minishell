/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 18:59:24 by wtang             #+#    #+#             */
/*   Updated: 2025/12/04 18:59:25 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*search_in_directory(char *dir, char *cmd)
{
	char	full_path[1024];

	ft_strlcpy(full_path, dir, sizeof(full_path));
	ft_strlcat(full_path, "/", sizeof(full_path));
	ft_strlcat(full_path, cmd, sizeof(full_path));
	if (access(full_path, X_OK) == 0)
		return (ft_strdup(full_path));
	return (NULL);
}

static char	*try_directory(char *current, char *cmd, char *end)
{
	char	saved_char;
	char	*result;

	saved_char = *end;
	*end = '\0';
	result = search_in_directory(current, cmd);
	*end = saved_char;
	return (result);
}

static char	*search_in_path(char *cmd, char *path_env)
{
	char	*path_copy;
	char	*current;
	char	*end;
	char	*result;

	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return (NULL);
	current = path_copy;
	while (*current)
	{
		end = current;
		while (*end && *end != ':')
			end++;
		result = try_directory(current, cmd, end);
		if (result)
			return (free(path_copy), result);
		if (*end == ':')
			current = end + 1;
		else
			current = end;
	}
	return (free(path_copy), NULL);
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
