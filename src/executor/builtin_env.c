#include "minishell.h"

int	builtin_env(t_command *cmd, t_env *env)
{
	int	i;
	(void)cmd;

	i = 0;
	if (cmd->argv[1])
	{
		write(2, "minishell: env: argument env only\n", 34);
		return (SYNTAX_ERROR);
	}
	while (i < env->count)
	{
		printf("%s\n", env->vars[i]);
		i++;
	}
	return (SUCCESS);
}
