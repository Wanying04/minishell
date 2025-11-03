#include "minishell.h"

void	update_pwd_oldpwd(char **PWD, char **OLDPWD, char *old_cwd)
{
	if (*OLDPWD)
		free (*OLDPWD);
	*OLDPWD = *PWD;
	*PWD = old_cwd;
}

char	*resolve_cd_path(char *arg, t_env *env, char *OLDPWD)
{
	char	*expanded;

	if (!arg || arg[0] == '\0')
		return (env_get(env, "HOME"));
	if (ft_strncmp(arg, "-", 2) == 0 && arg[2] == '\0')
		return (OLDPWD);
	expanded = expand_tilde(arg, env);
	if (expanded)
		return (expanded);
	else
		return (arg);
}

char	*get_cd_path(char *arg, t_env *env, char *OLDPWD, int *err)
{
	char	*path;

	path = resolve_cd_path(arg, env, OLDPWD);
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

int	builtin_cd_execute(char *arg, t_env *env, char **PWD, char **OLDPWD)
{
	char	*path;
	char	*old_cwd;
	int		ret;

	old_cwd = getcwd(NULL, 0);
	if (!old_cwd)
		return (perror("getcwd"), 1);
	path = get_cd_path(arg, env, *OLDPWD, &ret);
	if (!path)
		return (free(old_cwd), ret);
	if (chdir(path) != 0)
	{
		perror("cd");
		ret = 1;
	}
	else
	{
		update_pwd_oldpwd(PWD, OLDPWD, old_cwd);
		if (ft_strncmp(arg, "-", 2) == 0 && arg[2] == '\0')
			printf("%s\n", *PWD);
	}
	if (path && path != arg && path != *OLDPWD)
		free(path);
	return (ret);
}

int	builtin_cd(char **args, t_env *env)
{
	static char	*pwd;
	static char	*oldpwd;

	return (builtin_cd_execute(args[1], env, &pwd, &oldpwd));
}
