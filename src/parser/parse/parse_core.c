/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_core.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/10 18:43:26 by albarrei         ###   ########.fr       */
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
		free_redir_temp(ctx);
	if (ctx->heredoc_delim)
	{
		free(ctx->heredoc_delim);
		ctx->heredoc_delim = NULL;
	}
	return (ctx->error);
}

t_command	*parse_input(char *input, t_env *env)
{
	char	**tokens;
	t_pctx	ctx;

	ctx.heredoc_dont_expand = 0;
	tokens = ft_split_tokens(input, &ctx);
	if (!tokens)
	{
		env->exit_status = SYNTAX_ERROR;
		return (NULL);
	}
	init_ctx(&ctx, tokens, env);
	process_tokens_loop(&ctx);
	finalize_parsing(&ctx);
	ft_free_tokens(tokens);
	if (ctx.error)
	{
		env->exit_status = SYNTAX_ERROR;
		return (cleanup_resources(&ctx), NULL);
	}
	return (ctx.head);
}
