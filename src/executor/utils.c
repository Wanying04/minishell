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
		// Si el argumento NO está vacío, O si contiene marcadores de comillas (comillas vacías '')
		if (cmd->argv[i][0] != '\0' || 
			(ft_strlen(cmd->argv[i]) == 2 && 
			(cmd->argv[i][0] == '\x01' || cmd->argv[i][0] == '\x02')))
		{
			if (i != j)
				cmd->argv[j] = cmd->argv[i];
			j++;
		}
		else
		{
			// Liberar el argumento vacío que será removido
			free(cmd->argv[i]);
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
