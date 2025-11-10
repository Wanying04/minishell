#include "minishell.h"

/* Helpers kept short (<=25 lines), no for loops, use while only */

static int push_arg(char ***args_temp, int *args_count, int *args_cap, const char *tok)
{
	char **tmp;
	int z;
	int newcap;

	if (*args_count + 1 > *args_cap)
	{
		if (*args_cap)
			newcap = *args_cap * 2;
		else
			newcap = 8;
		tmp = malloc(sizeof(char *) * newcap);
		if (!tmp)
			return (1);
		z = 0;
		while (z < *args_count)
		{
			tmp[z] = (*args_temp)[z];
			z++;
		}
		free(*args_temp);
		*args_temp = tmp;
		*args_cap = newcap;
	}
	(*args_temp)[(*args_count)++] = ft_strdup(tok);
	return (0);
}

static int push_redir(t_redirect **redir_temp, int *redir_count, int *redir_cap, const char *file, int typeval)
{
	t_redirect *tmp;
	int z;
	int newcap;

	if (*redir_count + 1 > *redir_cap)
	{
		if (*redir_cap)
			newcap = *redir_cap * 2;
		else
			newcap = 4;
		tmp = malloc(sizeof(t_redirect) * newcap);
		if (!tmp)
			return (1);
		z = 0;
		while (z < *redir_count)
		{
			tmp[z] = (*redir_temp)[z];
			z++;
		}
		free(*redir_temp);
		*redir_temp = tmp;
		*redir_cap = newcap;
	}
	(*redir_temp)[*redir_count].file = ft_strdup(file);
	(*redir_temp)[*redir_count].type = typeval;
	(*redir_temp)[*redir_count].fd = -1;
	(*redir_count)++;
	return (0);
}

static void free_args_temp(char **args_temp, int args_count)
{
	int z;

	if (!args_temp)
		return;
	z = 0;
	while (z < args_count)
	{
		free(args_temp[z]);
		z++;
	}
	free(args_temp);
}

static t_command *create_node_from_temps(char **args_temp, int args_count, t_redirect *redir_temp, int redir_count, char *heredoc_delim)
{
	t_command *node;
	int j;

	node = malloc(sizeof(t_command));
	if (!node)
		return (NULL);
	node->argv = malloc(sizeof(char *) * (args_count + 1));
	if (!node->argv)
	{
		free(node);
		return (NULL);
	}
	j = 0;
	while (j < args_count)
	{
		node->argv[j] = args_temp[j];
		j++;
	}
	node->argv[args_count] = NULL;
	if (redir_count > 0)
	{
		node->redirects = redir_temp;
		node->redirect_count = redir_count;
	}
	else
	{
		node->redirects = NULL;
		node->redirect_count = 0;
		free(redir_temp);
	}
	node->heredoc_delimiter = heredoc_delim;
	node->is_builtin = 0;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

/* Create node and append to list; returns 0 on success, 1 on error */
static int finalize_and_append(t_command **head, t_command **curr, char **args_temp, int args_count, t_redirect *redir_temp, int redir_count, char *heredoc_delim)
{
	t_command *node;

	node = create_node_from_temps(args_temp, args_count, redir_temp, redir_count, heredoc_delim);
	if (!node)
		return (1);
	if (!*head)
		*head = node;
	if (*curr)
	{
		(*curr)->next = node;
		node->prev = *curr;
	}
	*curr = node;
	return (0);
}

static void cleanup_resources(char **args_temp, int args_count, t_redirect *redir_temp, int redir_count, char *heredoc_delim, t_command *head)
{
	if (args_temp)
		free_args_temp(args_temp, args_count);
	if (redir_temp)
	{
		int z;

		z = 0;
		while (z < redir_count)
		{
			if (redir_temp[z].file)
				free(redir_temp[z].file);
			z++;
		}
		free(redir_temp);
	}
	if (heredoc_delim)
		free(heredoc_delim);
	if (head)
		free_command(head);
}

static int handle_pipe(t_command **head, t_command **curr, char ***args_temp, int *args_count, int *args_cap, t_redirect **redir_temp, int *redir_count, int *redir_cap, char **heredoc_delim)
{
	if (*args_count == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		return (1);
	}
	if (finalize_and_append(head, curr, *args_temp, *args_count, *redir_temp, *redir_count, *heredoc_delim))
		return (1);
	/* reset temps (ownership moved into node where applicable) */
	*args_temp = NULL;
	*args_count = 0;
	*args_cap = 0;
	*heredoc_delim = NULL;
	*redir_temp = NULL;
	*redir_count = 0;
	*redir_cap = 0;
	return (0);
}

static int handle_redir(char **tokens, int *i, t_redirect **redir_temp, int *redir_count, int *redir_cap, char **heredoc_delim)
{
	char *tok;
	int type;
	int idx;

	tok = tokens[*i];
	type = 0;
	if (ft_strncmp(tok, ">>", 3) == 0)
		type = REDIR_APPEND;
	else if (ft_strncmp(tok, ">", 2) == 0)
		type = REDIR_OUT;
	else if (ft_strncmp(tok, "<<", 3) == 0)
		type = REDIR_HEREDOC;
	else if (ft_strncmp(tok, "<", 2) == 0)
		type = REDIR_IN;
	idx = *i + 1;
	if (!tokens[idx] || ft_isspecial(tokens[idx][0]))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'", 2);
		return (1);
	}
	if (type == REDIR_HEREDOC)
	{
		if (*heredoc_delim)
			free(*heredoc_delim);
		*heredoc_delim = ft_strdup(tokens[idx]);
	}
	else
	{
		if (push_redir(redir_temp, redir_count, redir_cap, tokens[idx], type))
			return (1);
	}
	/* leave *i at operator; caller will advance to after filename */
	return (0);
}

/* Debug printer for parsed command list */
void print_command_list(t_command *head)
{
	int idx;
	t_command *curr;
	int j;

	if (!head)
	{
		ft_putendl_fd("[parser] empty command list", 1);
		return;
	}
	idx = 0;
	curr = head;
	while (curr)
	{
		ft_putstr_fd("Command ", 1);
		ft_putnbr_fd(idx, 1);
		ft_putstr_fd(":\n  argv: ", 1);
		j = 0;
		if (curr->argv)
		{
			while (curr->argv[j])
			{
				ft_putstr_fd("\"", 1);
				ft_putstr_fd(curr->argv[j], 1);
				ft_putstr_fd("\" ", 1);
				j++;
			}
		}
		else
			ft_putstr_fd("(null)", 1);
		ft_putstr_fd("\n  heredoc: ", 1);
		if (curr->heredoc_delimiter)
			ft_putendl_fd(curr->heredoc_delimiter, 1);
		else
			ft_putendl_fd("(none)", 1);
		ft_putstr_fd("  redirects: ", 1);
		if (curr->redirects && curr->redirect_count > 0)
		{
			j = 0;
			while (j < curr->redirect_count)
			{
				if (curr->redirects[j].type == REDIR_OUT)
					ft_putstr_fd("OUT->", 1);
				else if (curr->redirects[j].type == REDIR_APPEND)
					ft_putstr_fd("APPEND->", 1);
				else if (curr->redirects[j].type == REDIR_IN)
					ft_putstr_fd("IN->", 1);
				else if (curr->redirects[j].type == REDIR_HEREDOC)
					ft_putstr_fd("HEREDOC->", 1);
				ft_putstr_fd("\"", 1);
				ft_putstr_fd(curr->redirects[j].file, 1);
				ft_putstr_fd("\" ", 1);
				j++;
			}
			ft_putstr_fd("\n", 1);
		}
		else
			ft_putendl_fd("none", 1);
		curr = curr->next;
		idx++;
	}
}

/*
 * parse_input - Función principal de análisis
 * 
 * Funcionalidad: Analizar la cadena de entrada del usuario en una lista de t_command
 * 
 * Entrada: char *input - Cadena de línea de comandos del usuario
 * Retorno: t_command * - Puntero a la cabeza de la lista de comandos, NULL si falla
 * 
 * TODO: El equipo Parser necesita implementación completa
 */
t_command	*parse_input(char *input)
{
	t_command	*head;
	t_command	*curr;
	char		**tokens;
	int			 i;
	int			 error;

	head = NULL;
	curr = NULL;
	tokens = ft_split_tokens(input);
	if (!tokens)
		return (NULL);

	i = 0;
	error = 0;
	/* Temporary containers for building a command */
	char **args_temp = NULL;
	int args_count = 0;
	int args_cap = 0;

	t_redirect *redir_temp = NULL;
	int redir_count = 0;
	int redir_cap = 0;

	char *heredoc_delim = NULL;

	/* helper functions implemented below */

	while (tokens[i])
	{
		char *tok = tokens[i];

		if (ft_strncmp(tok, "|", 2) == 0)
		{
			if (handle_pipe(&head, &curr, &args_temp, &args_count, &args_cap, &redir_temp, &redir_count, &redir_cap, &heredoc_delim))
			{
				error = 1;
				break;
			}
			i++;
			continue;
		}
		else if (ft_strncmp(tok, ">", 2) == 0 || ft_strncmp(tok, ">>", 3) == 0 || ft_strncmp(tok, "<", 2) == 0 || ft_strncmp(tok, "<<", 3) == 0)
		{
			if (handle_redir(tokens, &i, &redir_temp, &redir_count, &redir_cap, &heredoc_delim))
			{
				error = 1;
				break;
			}
			i++;
			continue;
		}
		else
		{
			if (push_arg(&args_temp, &args_count, &args_cap, tok))
			{
				error = 1;
				break;
			}
			i++;
			continue;
		}
	}

	/* finalize last command if present */
	if (!error && args_count > 0)
	{
		t_command *node = create_node_from_temps(args_temp, args_count, redir_temp, redir_count, heredoc_delim);
		if (!node)
			error = 1;
		if (!head)
			head = node;
		if (curr)
		{
			curr->next = node;
			node->prev = curr;
		}
		curr = node;
	}
	else
	{
		/* nothing to run (e.g., input was empty or only redirects/pipes) */
		if (redir_temp)
			free(redir_temp);
		if (heredoc_delim)
			free(heredoc_delim);
	}

	/* free original tokens (they were strdup'ed inside lexer) */
	ft_free_tokens(tokens);
	if (error)
	{
		cleanup_resources(args_temp, args_count, redir_temp, redir_count, heredoc_delim, head);
		return (NULL);
	}
	return (head);
}
