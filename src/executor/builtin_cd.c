#include "minishell.h"

static char	*remove_last_component(char *path)
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

static char	*build_logical_path(char *current_pwd, char *target)
{
	if (!current_pwd || !target)
		return (NULL);
	if (ft_strncmp(target, "..", 3) == 0 || !target[0])
		return (remove_last_component(current_pwd));
	return (NULL);
}

static void	set_env_var(t_env *env, const char *name, const char *value)
{
	char	*eq;
	char	*env_str;

	if (!value || !(eq = ft_strjoin(name, "=")))
		return ;
	env_str = ft_strjoin(eq, value);
	free(eq);
	if (env_str)
	{
		env_set(env, env_str);
		free(env_str);
	}
}

static void	update_pwd_oldpwd(t_env *env, char *old_cwd, char *new_cwd)
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

int	builtin_cd(t_command *cmd, t_env *env)
{
	char	*path;
	char	*old_cwd;
	char	*temp;
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
		return (free(old_cwd), ret);
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
	ret = 0;
	temp = getcwd(NULL, 0);
	if (!temp)
		temp = build_logical_path(env->pwd->pwd, cmd->argv[1]);
	update_pwd_oldpwd(env, old_cwd, temp);
	if (should_free)
		free(path);
	return (SUCCESS);
}
