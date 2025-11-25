/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	realloc_args(t_pctx *ctx)
{
	char	**tmp;
	int		z;
	int		newcap;

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
	return (0);
}

int	push_arg(t_pctx *ctx, const char *tok)
{
	char	*expanded;

	if (ctx->args_count + 1 > ctx->args_cap)
	{
		if (realloc_args(ctx))
			return (1);
	}
	expanded = expand_variables((char *)tok, ctx->env, 0);
	ctx->args_temp[ctx->args_count++] = expanded;
	return (0);
}

static int	realloc_redir(t_pctx *ctx)
{
	t_redirect	*tmp;
	int			z;
	int			newcap;

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
	return (0);
}

int	push_redir(t_pctx *ctx, const char *file, int typeval)
{
	char	*expanded;

	if (ctx->redir_count + 1 > ctx->redir_cap)
	{
		if (realloc_redir(ctx))
			return (1);
	}
	expanded = expand_variables((char *)file, ctx->env, 0);
	ctx->redir_temp[ctx->redir_count].file = expanded;
	ctx->redir_temp[ctx->redir_count].type = typeval;
	ctx->redir_temp[ctx->redir_count].fd = -1;
	ctx->redir_count++;
	return (0);
}
