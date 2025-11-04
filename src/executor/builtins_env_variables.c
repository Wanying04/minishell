#include "minishell.h"

int	is_valid_var_name(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

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
			printf("export: not a valid identifier\n");
			return (1);
		}
		env_set(env, cmd->argv[i]);
		i++;
	}
	return (0);
}

int	builtin_unset(t_command *cmd, t_env *env)
{
	int	i;

	if (!cmd->argv[1])
		return (0);
	i = 1;
	while (cmd->argv[i])
	{
		if (!is_valid_var_name(cmd->argv[i]))
		{
			printf("unset: not a valid identifier\n");
			return (1);
		}
		env_unset(env, cmd->argv[i]);
		i++;
	}
	return (0);
}
