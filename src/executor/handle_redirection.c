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
	if (analyze_redirections(cmd, &redir_config) != SUCCESS)
		return (FAILURE);
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
	int	final_input_exists;
	int	final_output_exists;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (FAILURE);
	final_input_exists = 0;
	final_output_exists = 0;
	if (cmd->redirects[cmd->redirect_count - 1].type == REDIR_IN || cmd->redirects[cmd->redirect_count - 1].type == REDIR_HEREDOC)
	{
		config->final_input = cmd->redirects[cmd->redirect_count - 1].file;
		i = 1;
		while (i < cmd->redirect_count)
		{
			if (cmd->redirects[cmd->redirect_count - 1 - i].type == REDIR_OUT || cmd->redirects[cmd->redirect_count - 1 - i].type == REDIR_APPEND)
			{
				config->final_output = cmd->redirects[cmd->redirect_count - 1 - i].file;
				final_output_exists = 1;
				break ;
			}
			i++;
		}
		if (!final_output_exists)
			config->final_output = NULL;
	}
	else
	{
		config->final_output = cmd->redirects[cmd->redirect_count - 1].file;
		i = 1;
		while (i < cmd->redirect_count)
		{
			if (cmd->redirects[cmd->redirect_count - 1 - i].type == REDIR_IN || cmd->redirects[cmd->redirect_count - 1].type == REDIR_HEREDOC)
			{
				config->final_input = cmd->redirects[cmd->redirect_count - 1 - i].file;
				final_input_exists = 1;
				break ;
			}
			i++;
		}
		if (!final_input_exists)
			config->final_input = NULL;
	}
	i = 0;
	while (i < cmd->redirect_count)
	{
		if (cmd->redirects[i].type == REDIR_HEREDOC)
		{
			config->delimiters[i] = cmd->redirects[i].file;
			config->delim_count++;
		}
		i++;
	}
}

int	execute_redirections(t_command *cmd, t_redir_config *config)
{

}

void	cleanup_redirections(t_redir_config *config)
{

}
