#include "minishell.h"

static char	*create_env_string(const char *name, const char *value)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (!result)
		return (NULL);
	i = 0;
	while (name[i])
	{
		result[i] = name[i];
		i++;
	}
	result[i++] = '=';
	j = 0;
	while (value[j])
		result[i++] = value[j++];
	result[i] = '\0';
	return (result);
}

void	update_pwd_oldpwd(t_env *env, char *old_cwd, char *new_cwd)
{
	char	*env_str;

	if (env->pwd->oldpwd)
		free(env->pwd->oldpwd);
	env->pwd->oldpwd = old_cwd;
	if (env->pwd->pwd)
		free(env->pwd->pwd);
	env->pwd->pwd = new_cwd;
	if (old_cwd)
	{
		env_str = create_env_string("OLDPWD", old_cwd);
		if (env_str)
		{
			env_set(env, env_str);
			free(env_str);
		}
	}
	if (new_cwd)
	{
		env_str = create_env_string("PWD", new_cwd);
		if (env_str)
		{
			env_set(env, env_str);
			free(env_str);
		}
	}
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
	else
		return (arg);
}

static char	*get_cd_path(char *arg, t_env *env, int *err, int *should_free)
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

static int	builtin_cd_execute(char *arg, t_env *env)
{
	char	*path;
	char	*old_cwd;
	int		ret;
	int		should_free;
	int		is_oldpwd;

	old_cwd = getcwd(NULL, 0);
	if (!old_cwd)
		return (perror("minishell: cd"), FAILURE);
	is_oldpwd = (arg && ft_strncmp(arg, "-", 2) == 0 && arg[1] == '\0');
	path = get_cd_path(arg, env, &ret, &should_free);
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
	else
	{
		char *new_cwd = getcwd(NULL, 0);
		if (is_oldpwd)
			printf("%s\n", path);
		update_pwd_oldpwd(env, old_cwd, new_cwd);
		if (should_free)
			free(path);
		ret = SUCCESS;
	}
	return (ret);
}

int	builtin_cd(t_command *cmd, t_env *env)
{
	return (builtin_cd_execute(cmd->argv[1], env));
}
