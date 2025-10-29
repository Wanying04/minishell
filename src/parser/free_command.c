#include "minishell.h"

/*
 * free_command - Liberar lista de comandos
 * 
 * Funcionalidad: Liberar recursivamente toda la lista t_command y toda la memoria asignada
 * 
 * Entrada: t_command *cmd - Puntero a la cabeza de la lista de comandos
 */
void	free_command(t_command *cmd)
{
	t_command	*next;
	int			i;

	while (cmd)
	{
		next = cmd->next;
		
		// Liberar argv
		if (cmd->argv)
			ft_free_tokens(cmd->argv);
		
		// Liberar redirects
		if (cmd->redirects)
		{
			i = 0;
			while (i < cmd->redirect_count)
			{
				if (cmd->redirects[i].file)
					free(cmd->redirects[i].file);
				i++;
			}
			free(cmd->redirects);
		}
		
		// Liberar heredoc_delimiter
		if (cmd->heredoc_delimiter)
			free(cmd->heredoc_delimiter);
		
		// Liberar la estructura del comando en sí
		free(cmd);
		cmd = next;
	}
}
