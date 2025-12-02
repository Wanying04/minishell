#include "minishell.h"

int	is_valid_pipeline(t_command *cmd)
{
	while (cmd->next && cmd->is_piped == 1)
	{
		if (!cmd || !cmd->argv || !cmd->argv[0])
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
			return (FAILURE);
		}
		cmd = cmd->next;
	}
	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	execute_pipeline(t_command *cmd, t_env *env)
{
	if (is_valid_pipeline(cmd) != SUCCESS)
		return (FAILURE);
	if (execute_pipeline_core(cmd, env) != SUCCESS)
		return (FAILURE);
	return (SUCCESS);
}
