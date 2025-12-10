/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ctx.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/10 22:33:34 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	free_redir_temp(t_pctx *ctx)
{
	int	z;

	if (!ctx->redir_temp)
		return ;
	z = 0;
	while (z < ctx->redir_count)
	{
		if (ctx->redir_temp[z].file)
			free(ctx->redir_temp[z].file);
		z++;
	}
	free(ctx->redir_temp);
	ctx->redir_temp = NULL;
	ctx->redir_count = 0;
	ctx->redir_cap = 0;
}

void	reset_ctx(t_pctx *ctx)
{
	ctx->heredoc_dont_expand = 0;
	if (ctx->args_temp)
		free_args_temp(ctx->args_temp, ctx->args_count);
	ctx->args_temp = NULL;
	ctx->args_count = 0;
	ctx->args_cap = 0;
	if (ctx->redir_temp)
		free_redir_temp(ctx);
	ctx->redir_temp = NULL;
	ctx->redir_count = 0;
	ctx->redir_cap = 0;
	if (ctx->heredoc_delim)
		free(ctx->heredoc_delim);
	ctx->heredoc_delim = NULL;
	ctx->is_in_pipe = 0;
}

void	init_ctx(t_pctx *ctx, char **tokens, t_env *env)
{
	if (tokens)
		ctx->tokens = tokens;
	ctx->env = env;
	ctx->args_temp = NULL;
	ctx->args_count = 0;
	ctx->args_cap = 0;
	ctx->redir_temp = NULL;
	ctx->redir_count = 0;
	ctx->redir_cap = 0;
	ctx->head = NULL;
	ctx->curr = NULL;
	ctx->error = 0;
	ctx->heredoc_delim = NULL;
	ctx->is_in_pipe = 0;
}

void	cleanup_resources(t_pctx *ctx)
{
	if (ctx->args_temp)
	{
		free_args_temp(ctx->args_temp, ctx->args_count);
		ctx->args_temp = NULL;
		ctx->args_count = 0;
		ctx->args_cap = 0;
	}
	if (ctx->redir_temp)
		free_redir_temp(ctx);
	if (ctx->heredoc_delim)
	{
		free(ctx->heredoc_delim);
		ctx->heredoc_delim = NULL;
	}
	if (ctx->head)
	{
		free_command(ctx->head);
		ctx->head = NULL;
		ctx->curr = NULL;
	}
}
