/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ctx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_ctx(t_pctx *ctx)
{
	ctx->args_temp = NULL;
	ctx->args_count = 0;
	ctx->args_cap = 0;
	ctx->heredoc_delim = NULL;
	ctx->redir_temp = NULL;
	ctx->redir_count = 0;
	ctx->redir_cap = 0;
	ctx->is_in_pipe = 0;
}

void	init_ctx(t_pctx *ctx, char **tokens, t_env *env)
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
	ctx->is_in_pipe = 0;
}

void	free_args_temp(char **args_temp, int args_count)
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

void	cleanup_resources(t_pctx *ctx)
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
