#include "minishell.h"

int	builtin_pwd(t_command *cmd)
{
	char	cwd[4096];

	(void)cmd;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("minishell: pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}
