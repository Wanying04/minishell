/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	finalize_parsing(t_pctx *ctx)
{
	if (!ctx->error && (ctx->args_count > 0 || ctx->redir_count > 0))
	{
		if (finalize_and_append(ctx))
			ctx->error = 1;
	}
	else if (ctx->redir_temp)
		free(ctx->redir_temp);
	if (ctx->heredoc_delim)
		free(ctx->heredoc_delim);
	return (ctx->error);
}

t_command	*parse_input(char *input, t_env *env)
{
	char	**tokens;
	t_pctx	ctx;

	tokens = ft_split_tokens(input);
	if (!tokens)
		return (NULL);
	init_ctx(&ctx, tokens, env);
	process_tokens_loop(&ctx);
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
	finalize_parsing(&ctx);
	ft_free_tokens(tokens);
	if (ctx.error)
		return (cleanup_resources(&ctx), NULL);
	return (ctx.head);
}
