#include "minishell.h"

int	execute(t_command *cmd, t_env *env)
{
	if (!cmd || !cmd->argv)
		return (SUCCESS);
	if (!cmd->argv[0] && cmd->redirect_count > 0)
		return (handle_redirections_only(cmd));
	if (!cmd->argv[0])
		return (SUCCESS);
	if (cmd->next != NULL)
		return (execute_pipeline(cmd, env));
	else
		return (execute_single_command(cmd, env));
}
