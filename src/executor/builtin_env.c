#include "minishell.h"

int	builtin_env(t_command *cmd, t_env *env)
{
	int	i;
	(void)cmd;

	i = 0;
	while (i < env->count)
	{
		printf("%s\n", env->vars[i]);
		i++;
	}
	return (0);
}
