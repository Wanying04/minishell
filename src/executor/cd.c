#include "minishell.h"

void	update_pwd_oldpwd(t_env *env, char *old_cwd, char *new_cwd)
{
	if (env->pwd->oldpwd)
		free(env->pwd->oldpwd);
	env->pwd->oldpwd = old_cwd;
	if (env->pwd->pwd)
		free(env->pwd->pwd);
	env->pwd->pwd = new_cwd;
}

char	*resolve_cd_path(char *arg, t_env *env, int *should_free)
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
	else
		return (arg);
}

char	*get_cd_path(char *arg, t_env *env, int *err, int *should_free)
{
	char	*path;

	if (arg && ft_strncmp(arg, "-", 2) == 0 && arg[1] == '\0' && !env->pwd->oldpwd)
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

int	builtin_cd_execute(char *arg, t_env *env)
{
	char	*path;
	char	*old_cwd;
	int		ret;
	int		should_free;

	old_cwd = getcwd(NULL, 0);
	if (!old_cwd)
		return (perror("getcwd"), 1);
	path = get_cd_path(arg, env, &ret, &should_free);
	if (!path)
		return (free(old_cwd), ret);
	if (chdir(path) != 0)
	{
		perror("cd");
		free(old_cwd);
		if (should_free)
			free(path);
		ret = 1;
	}
	else
	{
		char *new_cwd = getcwd(NULL, 0);
		update_pwd_oldpwd(env, old_cwd, new_cwd);
		if (should_free)
			free(path);
	}
	return (ret);
}

int	builtin_cd(t_command *cmd, t_env *env)
{
	return (builtin_cd_execute(cmd->argv[1], env));
}
