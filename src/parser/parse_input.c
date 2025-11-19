#include "minishell.h"

static void	reset_ctx(t_pctx *ctx)
{
	ctx->args_temp = NULL;
	ctx->args_count = 0;
	ctx->args_cap = 0;
	ctx->heredoc_delim = NULL;
	ctx->redir_temp = NULL;
	ctx->redir_count = 0;
	ctx->redir_cap = 0;
}

static int	push_arg(t_pctx *ctx, const char *tok)
{
	char	**tmp;
	char	*expanded;
	int		z;
	int		newcap;

	if (ctx->args_count + 1 > ctx->args_cap)
	{
		if (ctx->args_cap)
			newcap = ctx->args_cap * 2;
		else
			newcap = 8;
		tmp = malloc(sizeof(char *) * newcap);
		if (!tmp)
			return (1);
		z = 0;
		while (z < ctx->args_count)
		{
			tmp[z] = ctx->args_temp[z];
			z++;
		}
		free(ctx->args_temp);
		ctx->args_temp = tmp;
		ctx->args_cap = newcap;
	}
	// Expandir variables de entorno en el argumento
	expanded = expand_variables((char *)tok, ctx->env, 0);
	ctx->args_temp[ctx->args_count++] = expanded;
	return (0);
}

static int	push_redir(t_pctx *ctx, const char *file, int typeval)
{
	t_redirect	*tmp;
	char		*expanded;
	int			z;
	int			newcap;

	if (ctx->redir_count + 1 > ctx->redir_cap)
	{
		if (ctx->redir_cap)
			newcap = ctx->redir_cap * 2;
		else
			newcap = 4;
		tmp = malloc(sizeof(t_redirect) * newcap);
		if (!tmp)
			return (1);
		z = 0;
		while (z < ctx->redir_count)
		{
			tmp[z] = ctx->redir_temp[z];
			z++;
		}
		free(ctx->redir_temp);
		ctx->redir_temp = tmp;
		ctx->redir_cap = newcap;
	}
	// Expandir variables de entorno en el nombre del archivo
	expanded = expand_variables((char *)file, ctx->env, 0);
	ctx->redir_temp[ctx->redir_count].file = expanded;
	ctx->redir_temp[ctx->redir_count].type = typeval;
	ctx->redir_temp[ctx->redir_count].fd = -1;
	ctx->redir_count++;
	return (0);
}

static void	free_args_temp(char **args_temp, int args_count)
{
	int	z;

	if (!args_temp)
		return ;
	z = 0;
	while (z < args_count)
	{
		free(args_temp[z]);
		z++;
	}
	free(args_temp);
}

static t_command	*create_node_from_ctx(t_pctx *ctx)
{
	t_command	*node;
	int			j;

	node = malloc(sizeof(t_command));
	if (!node)
		return (NULL);
	node->argv = malloc(sizeof(char *) * (ctx->args_count + 1));
	if (!node->argv)
	{
		free(node);
		return (NULL);
	}
	j = 0;
	while (j < ctx->args_count)
	{
		node->argv[j] = ctx->args_temp[j];
		j++;
	}
	node->argv[ctx->args_count] = NULL;
	if (ctx->redir_count > 0)
	{
		node->redirects = ctx->redir_temp;
		node->redirect_count = ctx->redir_count;
		ctx->redir_temp = NULL;
		ctx->redir_count = 0;
		ctx->redir_cap = 0;
	}
	else
	{
		node->redirects = NULL;
		node->redirect_count = 0;
	}
	node->heredoc_delimiter = ctx->heredoc_delim;
	ctx->heredoc_delim = NULL;
	node->is_builtin = 0;
	node->next = NULL;
	node->prev = NULL;
	ctx->args_temp = NULL;
	ctx->args_count = 0;
	ctx->args_cap = 0;
	return (node);
}

static int	finalize_and_append(t_pctx *ctx)
{
	t_command	*node;

	node = create_node_from_ctx(ctx);
	if (!node)
		return (1);
	if (!ctx->head)
		ctx->head = node;
	if (ctx->curr)
	{
		ctx->curr->next = node;
		node->prev = ctx->curr;
	}
	ctx->curr = node;
	return (0);
}

static void	cleanup_resources(t_pctx *ctx)
{
	int	z;

	if (ctx->args_temp)
		free_args_temp(ctx->args_temp, ctx->args_count);
	if (ctx->redir_temp)
	{
		z = 0;
		while (z < ctx->redir_count)
		{
			if (ctx->redir_temp[z].file)
				free(ctx->redir_temp[z].file);
			z++;
		}
		free(ctx->redir_temp);
	}
	if (ctx->heredoc_delim)
		free(ctx->heredoc_delim);
	if (ctx->head)
		free_command(ctx->head);
}

static int	handle_semicolon(t_pctx *ctx)
{
	char	*next_tok;

	if (ctx->args_count == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `;'", 2);
		ctx->error = 1;
		return (1);
	}
	if (finalize_and_append(ctx))
		return (1);
	next_tok = ctx->tokens[ctx->i + 1];
	if (next_tok)
	{
		if (ft_strncmp(next_tok, "|", 2) == 0)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
			ctx->error = 1;
			return (1);
		}
		if (ft_strncmp(next_tok, ";", 2) == 0)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `;'", 2);
			ctx->error = 1;
			return (1);
		}
	}
	
	reset_ctx(ctx);
	return (0);
}

static int	handle_pipe(t_pctx *ctx)
{
	char	*next_tok;

	if (ctx->args_count == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		ctx->error = 1;
		return (1);
	}
	next_tok = ctx->tokens[ctx->i + 1];
	if (!next_tok)
	{
		ft_putendl_fd("minishell: syntax error: unexpected end of input", 2);
		ctx->error = 1;
		return (1);
	}
	if (ft_strncmp(next_tok, ";", 2) == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `;'", 2);
		ctx->error = 1;
		return (1);
	}
	if (ft_strncmp(next_tok, "|", 2) == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		ctx->error = 1;
		return (1);
	}
	if (finalize_and_append(ctx))
		return (1);
	reset_ctx(ctx);
	return (0);
}

static int	handle_redir(t_pctx *ctx)
{
	char	*tok;
	int		type;
	int		idx;

	tok = ctx->tokens[ctx->i];
	type = 0;
	if (ft_strncmp(tok, ">>", 3) == 0)
		type = REDIR_APPEND;
	else if (ft_strncmp(tok, ">", 2) == 0)
		type = REDIR_OUT;
	else if (ft_strncmp(tok, "<<", 3) == 0)
		type = REDIR_HEREDOC;
	else if (ft_strncmp(tok, "<", 2) == 0)
		type = REDIR_IN;
	idx = ctx->i + 1;
	if (!ctx->tokens[idx] || ft_isspecial(ctx->tokens[idx][0]))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token", 2);
		return (1);
	}
	if (type == REDIR_HEREDOC)
		ctx->heredoc_delim = ft_strdup(ctx->tokens[idx]);
	if (push_redir(ctx, ctx->tokens[idx], type))
			return (1);
	/* if (type == REDIR_HEREDOC)
	{
		if (ctx->heredoc_delim)
			free(ctx->heredoc_delim);
		ctx->heredoc_delim = ft_strdup(ctx->tokens[idx]);
	}
	else
	{
		if (push_redir(ctx, ctx->tokens[idx], type))
			return (1);
	} */
	return (0);
}

static void	print_redir_helper(t_command *curr, int j)
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
}

static void	print_cmd_argv(t_command *curr)
{
	int	j;

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
}

static void	print_cmd_redirs(t_command *curr)
{
	int	j;

	if (curr->redirects && curr->redirect_count > 0)
	{
		j = 0;
		while (j < curr->redirect_count)
		{
			print_redir_helper(curr, j);
			j++;
		}
		ft_putstr_fd("\n", 1);
	}
	else
		ft_putendl_fd("none", 1);
}

void	print_command_list(t_command *head)
{
	int			idx;
	t_command	*curr;

	if (!head)
	{
		ft_putendl_fd("[parser] empty command list", 1);
		return ;
	}
	idx = 0;
	curr = head;
	while (curr)
	{
		ft_putstr_fd("Command ", 1);
		ft_putnbr_fd(idx, 1);
		ft_putstr_fd(":\n  argv: ", 1);
		print_cmd_argv(curr);
		ft_putstr_fd("\n  heredoc: ", 1);
		if (curr->heredoc_delimiter)
			ft_putendl_fd(curr->heredoc_delimiter, 1);
		else
			ft_putendl_fd("(none)", 1);
		ft_putstr_fd("  redirects: ", 1);
		print_cmd_redirs(curr);
		curr = curr->next;
		idx++;
	}
}

static void	init_ctx(t_pctx *ctx, char **tokens, t_env *env)
{
	if (tokens)
		ctx->tokens = tokens;
	ctx->args_temp = NULL;
	ctx->args_count = 0;
	ctx->args_cap = 0;
	ctx->redir_temp = NULL;
	ctx->redir_count = 0;
	ctx->redir_cap = 0;
	ctx->heredoc_delim = NULL;
	ctx->head = NULL;
	ctx->curr = NULL;
	ctx->i = 0;
	ctx->error = 0;
	ctx->env = env;
	ctx->in_single_quotes = 0;
}

static int	process_token(t_pctx *ctx, char *tok)
{
    if (ft_strncmp(tok, ";;", 2) == 0)
        return (ft_putendl_fd("minishell: syntax error near unexpected token `;;'", 2),
            ctx->error = 1, 1);
    if (ft_strncmp(tok, ";", 2) == 0)
        return (handle_semicolon(ctx) || (ctx->i++, 0));
    if (ft_strncmp(tok, "|", 2) == 0)
        return (handle_pipe(ctx) || (ctx->i++, 0));
    if (ft_strncmp(tok, ">", 2) == 0 || ft_strncmp(tok, ">>", 3) == 0
        || ft_strncmp(tok, "<", 2) == 0 || ft_strncmp(tok, "<<", 3) == 0)
    {
        if (!ctx->tokens[ctx->i + 1])
            return (ft_putendl_fd("minishell: syntax error", 2),
                ctx->error = 1, 1);
        if (handle_redir(ctx))
            return (1);
        return (ctx->i += 2, 0);
    }
    if (push_arg(ctx, tok))
        return (1);
    return (ctx->i++, 0);
}

static void	parser_debug(t_pctx ctx)
{
	printf("\n\033[1;36m========== DEBUG PARSER ==========\033[0m\n");
	printf("\033[1;33m[TOKENS]\033[0m\n");
	for (int i = 0; ctx.tokens[i] != NULL; i++)
	{
		printf("  tokens[%d] = \"%s\"\n", i, ctx.tokens[i]);
		if (ctx.tokens[i + 1] == NULL)
			printf("  tokens[%d] = NULL\n", i + 1);
	}
	printf("\n\033[1;33m[CTX - args_temp] count=%d, cap=%d\033[0m\n",
		ctx.args_count, ctx.args_cap);
	if (ctx.args_temp)
	{
		for (int i = 0; i < ctx.args_count; i++)
			printf("  args_temp[%d] = \"%s\"\n", i, ctx.args_temp[i]);
	}
	else
		printf("  (null)\n");
	printf("\n\033[1;33m[CTX - redir_temp] count=%d, cap=%d\033[0m\n",
		ctx.redir_count, ctx.redir_cap);
	if (ctx.redir_temp)
	{
		for (int i = 0; i < ctx.redir_count; i++)
			printf("  redir[%d]: type=%d, file=\"%s\"\n", i,
				ctx.redir_temp[i].type, ctx.redir_temp[i].file);
	}
	else
		printf("  (null)\n");
	printf("\n\033[1;33m[CTX - heredoc_delim]\033[0m\n");
	printf("  %s\n", ctx.heredoc_delim ? ctx.heredoc_delim : "(null)");
	printf("\n\033[1;33m[CTX - head]\033[0m\n");
	printf("  %p\n", (void *)ctx.head);
	printf("\n\033[1;33m[CTX - curr]\033[0m\n");
	printf("  %p\n", (void *)ctx.curr);
	printf("\n\033[1;33m[CTX - i]\033[0m\n");
	printf("  %d\n", ctx.i);
	printf("\n\033[1;33m[CTX - error]\033[0m\n");
	printf("  %d\n", ctx.error);
	printf("\033[1;36m==================================\033[0m\n\n");
}
// ctx = context
// t_pctx = type parser context
t_command	*parse_input(char *input, t_env *env)
{
	char	**tokens;
	t_pctx	ctx;

	tokens = ft_split_tokens(input);
	if (!tokens)
		return (NULL);
	init_ctx(&ctx, tokens, env);
	while (ctx.tokens[ctx.i])
	{
		if (process_token(&ctx, ctx.tokens[ctx.i]))
		{
			ctx.error = 1;
			break ;
		}
	}
	// Comprobación de tokens -------
	if (getenv("PARSER_DEBUG2"))
	{
		parser_debug(ctx);
		ft_free_tokens(tokens);
		if (ctx.error)
			return (cleanup_resources(&ctx), NULL);
		if (ctx.head)
			free_command(ctx.head);
		return (NULL);
	}
	// Hasta aquí -------------------
	if (!ctx.error && (ctx.args_count > 0 || ctx.redir_count > 0))
	{
		if (finalize_and_append(&ctx))
			ctx.error = 1;
	}
	else if (ctx.redir_temp)
		free(ctx.redir_temp);
	if (ctx.heredoc_delim)
		free(ctx.heredoc_delim);
	ft_free_tokens(tokens);
	if (ctx.error)
		return (cleanup_resources(&ctx), NULL);
	return (ctx.head);
}
