#include "minishell.h"

int	builtin_pwd(t_command *cmd)
{
	char	cwd[4096];

	(void)cmd;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: pwd");
		return (FAILURE);
	}
	write(1, cwd, ft_strlen(cwd));
	write(1, "\n", 1);
	return (SUCCESS);
}
