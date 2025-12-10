/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/10 18:43:26 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy_argv_to_node(t_command *node, t_pctx *ctx)
{
	int	j;

	j = 0;
	while (j < ctx->args_count)
	{
		node->argv[j] = ctx->args_temp[j];
		j++;
	}
	node->argv[ctx->args_count] = NULL;
	free(ctx->args_temp);
}

static void	set_redirects(t_command *node, t_pctx *ctx)
{
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
}

t_command	*create_node_from_ctx(t_pctx *ctx)
{
	t_command	*node;

	node = ft_calloc(sizeof(t_command), 1);
	if (!node)
		return (NULL);
	node->argv = ft_calloc(sizeof(char *), (ctx->args_count + 1));
	if (!node->argv)
		return (free(node), NULL);
	copy_argv_to_node(node, ctx);
	set_redirects(node, ctx);
	node->heredoc_delimiter = ctx->heredoc_delim;
	node->dont_expand = ctx->heredoc_dont_expand;
	ctx->heredoc_delim = NULL;
	node->is_builtin = 0;
	node->is_piped = ctx->is_in_pipe;
	node->next = NULL;
	node->prev = NULL;
	ctx->args_temp = NULL;
	ctx->args_count = 0;
	ctx->args_cap = 0;
	return (node);
}

int	finalize_and_append(t_pctx *ctx)
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
