/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: albarrei <albarrei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 15:19:26 by albarrei          #+#    #+#             */
/*   Updated: 2025/12/10 18:43:26 by albarrei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill_tokens(char **tokens, char *input, t_pctx *ctx)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (ft_isspecial(input[i]))
			tokens[k++] = ft_get_special_token(input, &i);
		else
		{
			if (k > 0 && ft_strncmp(tokens[k - 1], "<<", 3) == 0
				&& (input[i] == '"' || input[i] == '\''))
				ctx->heredoc_dont_expand = 1;
			tokens[k++] = ft_get_normal_token(input, &i);
		}
	}
	tokens[k] = NULL;
}

char	**ft_split_tokens(char *input, t_pctx *ctx)
{
	char	**tokens;
	int		token_count;

	if (!input)
		return (NULL);
	if (ft_check_quotes(input))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes", 2);
		return (NULL);
	}
	token_count = ft_count_tokens(input);
	tokens = ft_calloc(sizeof(char *), (token_count + 1));
	if (!tokens)
		return (NULL);
	fill_tokens(tokens, input, ctx);
	return (tokens);
}
