/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wtang <wtang@student.42malaga.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 21:00:31 by wtang             #+#    #+#             */
/*   Updated: 2025/12/10 22:38:48 by wtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*resolve_cd_path(char *arg, t_env *env, int *should_free)
{
	char	*expanded;

	*should_free = 0;
	if (!arg || arg[0] == '\0')
		return (env_get(env, "HOME"));
	if (ft_strncmp(arg, "-", 2) == 0 && arg[1] == '\0')
		return (env->pwd->oldpwd);
	expanded = expand_tilde(arg, env);
	if (expanded && expanded != arg)
		*should_free = 1;
	if (expanded)
		return (expanded);
	return (arg);
}

static char	*get_cd_path(char *arg, t_env *env, int *err, int *should_free)
{
	char	*path;

	if (arg && ft_strncmp(arg, "-", 2) == 0 && arg[1] == '\0'
		&& !env->pwd->oldpwd)
	{
		printf("cd: OLDPWD not set\n");
		*err = 1;
		return (NULL);
	}
	path = resolve_cd_path(arg, env, should_free);
	if (!path)
	{
		if (!arg || arg[0] == '\0')
			printf("cd: HOME not set\n");
		else
			printf("cd: OLDPWD not set\n");
		*err = 1;
		return (NULL);
	}
	*err = 0;
	return (path);
}

static int	do_cd_and_update_env(char *path, char *old_cwd,
	t_command *cmd, t_env *env)
{
	char	*temp;
	int		should_free;

	should_free = 0;
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		free(old_cwd);
		if (should_free)
			free(path);
		return (FAILURE);
	}
	if (cmd->argv[1] && ft_strncmp(cmd->argv[1], "-", 2) == 0
		&& cmd->argv[1][1] == '\0')
		printf("%s\n", path);
	temp = getcwd(NULL, 0);
	if (!temp)
		temp = build_logical_path(env->pwd->pwd, cmd->argv[1]);
	update_pwd_oldpwd(env, old_cwd, temp);
	if (should_free)
		free(path);
	return (SUCCESS);
}

int	builtin_cd(t_command *cmd, t_env *env)
{
	char	*path;
	char	*old_cwd;
	int		ret;
	int		should_free;

	if (cmd->argv[1] && cmd->argv[2])
		return (write(2, "minishell: cd: too many arguments\n", 34), FAILURE);
	old_cwd = getcwd(NULL, 0);
	if (!old_cwd && env && env->pwd && env->pwd->pwd)
		old_cwd = ft_strdup(env->pwd->pwd);
	if (!old_cwd)
		return (perror("minishell: cd"), FAILURE);
	path = get_cd_path(cmd->argv[1], env, &ret, &should_free);
	if (!path)
	{
		free(old_cwd);
		return (ret);
	}
	return (do_cd_and_update_env(path, old_cwd, cmd, env));
}
