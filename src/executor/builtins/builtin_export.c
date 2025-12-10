#include "minishell.h"

static void	print_export_format(t_env *env)
{
	int		i;
	char	*equal_sign;
	char	*key;
	char	*value;

	i = 0;
	while (i < env->count)
	{
		equal_sign = ft_strchr(env->vars[i], '=');
		if (equal_sign)
		{
			key = ft_substr(env->vars[i], 0, equal_sign - env->vars[i]);
			value = equal_sign + 1;
			printf("declare -x %s=\"%s\"\n", key, value);
			free(key);
		}
		else
			printf("declare -x %s\n", env->vars[i]);
		i++;
	}
}

int	builtin_export(t_command *cmd, t_env *env)
{
	int	i;

	if (!cmd->argv[1])
	{
		print_export_format(env);
		return (SUCCESS);
	}
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
