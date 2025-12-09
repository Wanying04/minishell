#include "minishell.h"

int	builtin_export(t_command *cmd, t_env *env)
{
	int	i;

	if (!cmd->argv[1])
		return (builtin_env(cmd, env));
	i = 1;
	while (cmd->argv[i])
	{
		if (!is_valid_var_name(cmd->argv[i]))
		{
			write(2, "minishell: export: `", 20);
			write(2, cmd->argv[i], ft_strlen(cmd->argv[i]));
			write(2, "' not a valid identifier\n", 26);
			return (FAILURE);
		}
		if (ft_strchr(cmd->argv[i], '='))
			env_set(env, cmd->argv[i]);
		i++;
	}
	return (SUCCESS);
}
