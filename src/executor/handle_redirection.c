#include "minishell.h"

# define REDIR_IN 1
# define REDIR_OUT 2
# define REDIR_APPEND 3
# define REDIR_HEREDOC 4

typedef struct s_command
{
	char	**argv;
	int		is_builtin;
	int		redirect_count;
	t_redirect	*redirects;
	struct s_command	*prev;
	struct s_command	*next;
}	t_command;

typedef struct s_redirect
{
	int		type;
	char	*file;
}	t_redirect;

typedef struct s_redir_config
{
	char	*final_input;
	char	*final_output;
	char	**delimiters;
	int		output_mode;
	int		delim_count;
}	t_redir_config;

int	handle_redirections(t_command *cmd)
{
	t_redir_config	redir_config;

	ft_memset(&redir_config, 0, sizeof(redir_config));
	redir_config.delimiters = malloc(sizeof(char *) * cmd->redirect_count);
	if (!redir_config.delimiters)
		return (FAILURE);
	if (analyze_redirections(cmd, &redir_config) != SUCCESS)
	{
		free(redir_config.delimiters);
		return (FAILURE);
	}
	if (execute_redirections(cmd, &redir_config) != SUCCESS)
	{
		cleanup_redirections(&redir_config);
		return (FAILURE);
	}
	cleanup_redirections(&redir_config);
	return (SUCCESS);
}

int	analyze_redirections(t_command *cmd, t_redir_config *config)
{
	int	i;
	int	delim_index;

	if (!cmd || !config)
		return (FAILURE);
	i = cmd->redirect_count - 1;
	delim_index = 0;
	while (i >= 0)
	{
		if (!config->final_input && (cmd->redirects[i].type == REDIR_IN || cmd->redirects[i].type == REDIR_HEREDOC))
			config->final_input = cmd->redirects[i].file;
		if (!config->final_output && (cmd->redirects[i].type == REDIR_OUT || cmd->redirects[i].type == REDIR_APPEND))
		{
			config->final_output = cmd->redirects[i].file;
			config->output_mode = (cmd->redirects[i].type == REDIR_APPEND);
		}
		i--;
	}
	i = 0;
	while (i < cmd->redirect_count)
	{
		if (cmd->redirects[i].type == REDIR_HEREDOC)
		{
			config->delimiters[delim_index] = cmd->redirects[i].file;
			config->delim_count++;
			delim_index++;
		}
		i++;
	}
	return (SUCCESS);
}

int	execute_redirections(t_command *cmd, t_redir_config *config)
{

}

void	cleanup_redirections(t_redir_config *config)
{

}
