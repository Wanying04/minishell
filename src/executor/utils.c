#include "minishell.h"
#include <sys/stat.h>

void	cleanup_empty_args(t_command *cmd)
{
	int	i;
	int	j;

	if (!cmd || !cmd->argv)
		return ;
	i = 0;
	j = 0;
	while (cmd->argv[i])
	{
		if (cmd->argv[i][0] != '\0')
		{
			if (i != j)
				cmd->argv[j] = cmd->argv[i];
			j++;
		}
		i++;
	}
	cmd->argv[j] = NULL;
}

int	is_a_directory(const char *path)
{
	struct stat	path_stat;

	if (!path)
		return (0);
	if (stat(path, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}
