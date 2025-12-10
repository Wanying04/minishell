/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/10 22:33:39 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_pipe_syntax(t_pctx *ctx)
{
	char	*next_tok;

	next_tok = ctx->tokens[ctx->i + 1];
	if (!next_tok)
	{
		ft_putendl_fd("minishell: syntax error: unexpected end of input", 2);
		ctx->error = 2;
		return (2);
	}
	if (ft_strncmp(next_tok, "|", 2) == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		ctx->error = 2;
		return (2);
	}
	return (0);
}

int	handle_pipe(t_pctx *ctx)
{
	if (ctx->args_count == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		ctx->error = 2;
		return (2);
	}
	if (check_pipe_syntax(ctx))
		return (2);
	ctx->is_in_pipe = 1;
	if (finalize_and_append(ctx))
		return (1);
	reset_ctx(ctx);
	return (0);
}

static int	get_redir_type(char *tok)
{
	if (ft_strncmp(tok, ">>", 3) == 0)
		return (REDIR_APPEND);
	else if (ft_strncmp(tok, ">", 2) == 0)
		return (REDIR_OUT);
	else if (ft_strncmp(tok, "<<", 3) == 0)
		return (REDIR_HEREDOC);
	else if (ft_strncmp(tok, "<", 2) == 0)
		return (REDIR_IN);
	return (0);
}

int	handle_redir(t_pctx *ctx)
{
	char	*tok;
	int		type;
	int		idx;

	tok = ctx->tokens[ctx->i];
	type = get_redir_type(tok);
	idx = ctx->i + 1;
	if (!ctx->tokens[idx] || ft_isspecial(ctx->tokens[idx][0]))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token", 2);
		ctx->error = 2;
		return (2);
	}
	if (type == REDIR_HEREDOC)
		ctx->heredoc_delim = ft_strdup(ctx->tokens[idx]);
	if (push_redir(ctx, ctx->tokens[idx], type))
		return (1);
	return (0);
}
