#include "minishell.h"

int	builtin_pwd(t_command *cmd, t_env *env)
{
	char	*pwd;
	char	cwd[4096];

	(void)cmd;
	if (env && env->pwd && env->pwd->pwd)
	{
		pwd = env->pwd->pwd;
		write(1, pwd, ft_strlen(pwd));
		write(1, "\n", 1);
		return (SUCCESS);
	}
	else
	{
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("minishell: pwd");
			return (FAILURE);
		}
		write(1, cwd, ft_strlen(cwd));
		write(1, "\n", 1);
		return (SUCCESS);
	}
}
