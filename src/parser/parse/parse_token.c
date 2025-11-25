/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_semicolon(t_pctx *ctx, char *tok)
{
	if (ft_strncmp(tok, ";;", 2) == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `;;'", 2);
		ctx->error = 2;
		return (2);
	}
	if (ft_strncmp(tok, ";", 2) == 0)
		return (handle_semicolon(ctx) || (ctx->i++, 0));
	return (-1);
}

static int	process_pipe(t_pctx *ctx, char *tok)
{
	if (ft_strncmp(tok, "|", 2) == 0)
		return (handle_pipe(ctx) || (ctx->i++, 0));
	return (-1);
}

static int	process_redirection(t_pctx *ctx, char *tok)
{
	if (ft_strncmp(tok, ">", 2) == 0 || ft_strncmp(tok, ">>", 3) == 0
		|| ft_strncmp(tok, "<", 2) == 0 || ft_strncmp(tok, "<<", 3) == 0)
	{
		if (!ctx->tokens[ctx->i + 1])
		{
			ft_putendl_fd("minishell: syntax error", 2);
			ctx->error = 2;
			return (2);
		}
		if (handle_redir(ctx))
			return (1);
		return (ctx->i += 2, 0);
	}
	return (-1);
}

int	process_token(t_pctx *ctx, char *tok)
{
	int	result;

	result = process_semicolon(ctx, tok);
	if (result != -1)
		return (result);
	result = process_pipe(ctx, tok);
	if (result != -1)
		return (result);
	result = process_redirection(ctx, tok);
	if (result != -1)
		return (result);
	if (push_arg(ctx, tok))
		return (1);
	return (ctx->i++, 0);
}

void	process_tokens_loop(t_pctx *ctx)
{
	while (ctx->tokens[ctx->i])
	{
		if (process_token(ctx, ctx->tokens[ctx->i]))
		{
			if (ctx->error == 0)
				ctx->error = 1;
			break ;
		}
	}
}
