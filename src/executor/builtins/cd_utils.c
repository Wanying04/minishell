/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 21:00:27 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 21:00:28 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_last_component(char *path)
{
	char	*last_slash;
	char	*result;

	if (!path || !*path)
		return (NULL);
	if (ft_strncmp(path, "/", 2) == 0)
		return (ft_strdup("/"));
	last_slash = ft_strrchr(path, '/');
	if (!last_slash)
		return (ft_strdup("."));
	if (last_slash == path)
		return (ft_strdup("/"));
	result = malloc(last_slash - path + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, path, last_slash - path + 1);
	return (result);
}

char	*build_logical_path(char *current_pwd, char *target)
{
	if (!current_pwd || !target)
		return (NULL);
	if (ft_strncmp(target, "..", 3) == 0 || !target[0])
		return (remove_last_component(current_pwd));
	return (NULL);
}

void	set_env_var(t_env *env, const char *name, const char *value)
{
	char	*eq;
	char	*env_str;

	eq = ft_strjoin(name, "=");
	if (!value || !eq)
		return ;
	env_str = ft_strjoin(eq, value);
	free(eq);
	if (env_str)
	{
		env_set(env, env_str);
		free(env_str);
	}
}

void	update_pwd_oldpwd(t_env *env, char *old_cwd, char *new_cwd)
{
	if (env->pwd->oldpwd)
		free(env->pwd->oldpwd);
	env->pwd->oldpwd = old_cwd;
	if (env->pwd->pwd)
		free(env->pwd->pwd);
	env->pwd->pwd = new_cwd;
	set_env_var(env, "OLDPWD", old_cwd);
	set_env_var(env, "PWD", new_cwd);
}
