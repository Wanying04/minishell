/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_semicolon_syntax(t_pctx *ctx)
{
	char	*next_tok;

	next_tok = ctx->tokens[ctx->i + 1];
	if (next_tok)
	{
		if (ft_strncmp(next_tok, "|", 2) == 0)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|'",
				2);
			ctx->error = 2;
			return (2);
		}
		if (ft_strncmp(next_tok, ";", 2) == 0)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `;'",
				2);
			ctx->error = 2;
			return (2);
		}
	}
	return (0);
}

int	handle_semicolon(t_pctx *ctx)
{
	if (ctx->args_count == 0)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `;'", 2);
		ctx->error = 2;
		return (2);
	}
	if (finalize_and_append(ctx))
		return (1);
	if (check_semicolon_syntax(ctx))
		return (2);
	reset_ctx(ctx);
	return (0);
}
