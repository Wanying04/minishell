/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:29 by albarrei          #+#    #+#             */
/*   Updated: 2025/11/25 15:38:45 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	debug_tokens(t_pctx ctx)
{
	int	i;

	printf("\n\033[1;36m========== DEBUG PARSER ==========\033[0m\n");
	printf("\033[1;33m[TOKENS]\033[0m\n");
	i = 0;
	while (ctx.tokens[i] != NULL)
	{
		printf("  tokens[%d] = \"%s\"\n", i, ctx.tokens[i]);
		i++;
	}
	printf("  tokens[%d] = NULL\n", i);
}

static void	debug_args(t_pctx ctx)
{
	int	i;

	printf("\n\033[1;33m[CTX - args_temp] count=%d, cap=%d\033[0m\n",
		ctx.args_count, ctx.args_cap);
	if (ctx.args_temp)
	{
		i = 0;
		while (i < ctx.args_count)
		{
			printf("  args_temp[%d] = \"%s\"\n", i, ctx.args_temp[i]);
			i++;
		}
	}
	else
		printf("  (null)\n");
}

static void	debug_redirs(t_pctx ctx)
{
	int	i;

	printf("\n\033[1;33m[CTX - redir_temp] count=%d, cap=%d\033[0m\n",
		ctx.redir_count, ctx.redir_cap);
	if (ctx.redir_temp)
	{
		i = 0;
		while (i < ctx.redir_count)
		{
			printf("  redir[%d]: type=%d, file=\"%s\"\n", i,
				ctx.redir_temp[i].type, ctx.redir_temp[i].file);
			i++;
		}
	}
	else
		printf("  (null)\n");
}

static void	debug_rest(t_pctx ctx)
{
	char	*delim_str;

	printf("\n\033[1;33m[CTX - heredoc_delim]\033[0m\n");
	if (ctx.heredoc_delim)
		delim_str = ctx.heredoc_delim;
	else
		delim_str = "(null)";
	printf("  %s\n", delim_str);
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

void	parser_debug(t_pctx ctx)
{
	debug_tokens(ctx);
	debug_args(ctx);
	debug_redirs(ctx);
	debug_rest(ctx);
}
